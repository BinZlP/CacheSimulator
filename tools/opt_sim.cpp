#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

bool *reference_map;


// Function to check whether a page exists
// in a frame or not
bool search(int key, vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}

// Function to find the frame that will not be used
// recently in future after given index in pg[0..pn-1]
int predict(vector<int>& pg, vector<int>& fr, int pn, int index)
{
    // Store the index of pages which are going
    // to be used recently in future
    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }

        // If a page is never referenced in future,
        // return it.
        if (j == pn)
            return i;
    }

    // If all of the frames were not in future,
    // return any of them, we return 0. Otherwise
    // we return res.
    return (res == -1) ? 0 : res;
}

void optimalPage(vector<int> &pg, int pn, int fn)
{
    // Create an array for given number of
    // frames and initialize it as empty.
    vector<int> fr;

    // Traverse through page reference array
    // and check for miss and hit.
    int hit = 0;
    int miss = 0;
    for (int i = 0; i < pn; i++) {

        // Page found in a frame : HIT
        if (search(pg[i], fr)) {
            hit++;
            continue;
        }

        // Page not found in a frame : MISS
        if (reference_map[pg[i]]) miss++;
        else reference_map[pg[i]]=true;

        // If there is space available in frames.
        if (fr.size() < fn)
            fr.push_back(pg[i]);

        // Find the page to be replaced.
        else {
            int j = predict(pg, fr, pn, i + 1);
            fr[j] = pg[i];
        }
    }

    cout << "[Cache statistics]" << endl;
    cout << "  Cache hit: " << hit << endl;
    cout << "  Cache miss: " << miss << endl;
    cout << "  Cache hit ratio: " << (double)hit / (double)(hit+miss) << endl;
}

// Driver Function
int main(int argc, char** argv)
{
    if(argc < 3){
        cout << "[Usage]: " << argv[0] << " [log file] [block # of cache]" << endl;
        return -1;
    }

    vector<int> pg;
    string line;
    ifstream log_file(argv[1]);
    unsigned long long count = 0;
    while(getline(log_file, line)) {
        pg.push_back(stoull(line));
        count++;
    }

    cout << "Total access: " << count << endl;

    unsigned long long max_num = 0;
    for(unsigned long long i = 0; i<count; i++)
        if(max_num < pg[i]) max_num = pg[i];
    cout << "Max num: " << max_num << endl;

    reference_map = (bool *)calloc(max_num+1, sizeof(bool));

    int pn = pg.size();
    int fn = atoi(argv[2]);

    optimalPage(pg, pn, fn);
    return 0;
}
