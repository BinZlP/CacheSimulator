#pragma once

template<class T, class U>
class Hasher {
private:

public:
    virtual T hash(U key) = 0;
};