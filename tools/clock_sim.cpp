#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

bool *is_referenced;

// If page found, updates the second chance bit to true
static bool findAndUpdate(int x,int arr[],
                bool second_chance[],int frames)
 
{
    int i;
     
    for(i = 0; i < frames; i++)
    {
         
        if(arr[i] == x)
        {
            // Mark that the page deserves a second chance
            second_chance[i] = true;
             
            // Return 'true', that is there was a hit
            // and so there's no need to replace any page
            return true;
        }
    }
     
    // Return 'false' so that a page for replacement is selected
    // as he reuested page doesn't exist in memory
    return false;
     
}
 
 
// Updates the page in memory and returns the pointer
static int replaceAndUpdate(int x,int arr[],
            bool second_chance[],int frames,int pointer)
{
    while(true)
    {
         
        // We found the page to replace
        if(!second_chance[pointer])
        {
            // Replace with new page
            arr[pointer] = x;
             
            // Return updated pointer
            return (pointer + 1) % frames;
        }
         
        // Mark it 'false' as it got one chance
        // and will be replaced next time unless accessed again
        second_chance[pointer] = false;
         
        //Pointer is updated in round robin manner
        pointer = (pointer + 1) % frames;
    }
}
 
static void printHitsAndFaults(string reference_string,
                                            int frames)
{
    int pointer, i, l=0, x;
    unsigned long long hit = 0, cold_miss = 0, miss = 0;
     
    //initially we consider frame 0 is to be replaced
    pointer = 0;
     
    // Create a array to hold page numbers
    int arr[frames];
     
    // No pages initially in frame,
    // which is indicated by -1
    for(int i=0; i<frames; i++) arr[i] = -1;
     
    // Create second chance array.
    // Can also be a byte array for optimizing memory
    bool second_chance[frames];
     
    // Split the string into tokens,
    // that is page numbers, based on space
     
    vector<string> strs;
    string word = "";
    for (auto x : reference_string)
    {
        if (x == ' ')
        {
            strs.push_back(word);
            word = "";
            l++;
        }
        else
        {
            word = word + x;
        }
    }
    strs.push_back(word);
    l++;
    // l=the length of array
     
    for(i = 0; i < l; i++)
    {
        x = stoi(strs.at(i));
        
        // Finds if there exists a need to replace
        // any page at all
        if(!findAndUpdate(x,arr,second_chance,frames))
        {
            // Selects and updates a victim page
            pointer = replaceAndUpdate(x,arr,
                    second_chance,frames,pointer);
             
            // Update page faults
            if(is_referenced[x]) miss++;
            else { cold_miss++; is_referenced[x] = true; }
        } else hit++;
    }
    
    cout << "[Cache statistics]" << endl;
    cout << "  Cache hit: " << hit << endl;
    cout << "  Cache miss(w/o cold miss): " << miss << endl;
    cout << "  Cache cold miss: " << cold_miss << endl;
    cout << "  Cache hit ratio: " << (double)hit / (double)(hit+miss) << endl;
}
 
// Driver code
int main(int argc, char** argv)
{
    if(argc < 3){
        cout << "[Usage]: " << argv[0] << " [log file] [block # of cache]" << endl;
        return -1;
    }
    ifstream log_file(argv[1]);
    string line;
    string reference_string = "";
    int frames = atoi(argv[2]);
    unsigned long long count = 0;
    unsigned long long temp;
    unsigned long long max_num = 0;

    while(getline(log_file, line)) {
        temp = stoull(line);
        reference_string += to_string(temp);
        reference_string += " ";
        count++;

        if(temp > max_num) max_num = temp;
    }
    reference_string.pop_back();

    cout << "Offset max num: " << max_num << endl;
    is_referenced = new bool[max_num+1];
    for(unsigned long long i = 0; i<=max_num; i++)
        is_referenced[i] = 0;
    
    printHitsAndFaults(reference_string,frames);
    return 0;
}
 