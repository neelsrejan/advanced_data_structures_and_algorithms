#ifndef BSA_HPP
#define BSA_HPP

#include <vector>
using namespace std;

template<typename Data>

/** This class implements a binary search array */
class BSA {
    
    private:
        vector<Data> v;

        /** Helper method used to either find an element if it exists
         * or else find where it should be inserted.
         */
        virtual size_t binarySearch(const Data& item) const {
              
        }

    public:
        /** Return the position of item, otherwise v.cend() */
        virtual typename vector<Data>::const_iterator find(const Data& item) const {
            return v.begin();
            /*int itemPosition = 0;
            //for (int i = v.begin(); i < v.end(); i++) {
            for (int i = v.begin(); i < v.end(); i++) {
                if (v[i] == item) {
                    itemPosition = i;
                    return itemPosition;
                } else {
                    itemPosition = v.cend();
                    return itemPosition;
                }
            }*/
        }

        /** Insert item into sorted position */
        virtual bool insert(const Data& item) {
            return false;
            /*
            int indexToInsert = 0;
            for (int i = v.begin(); i < v.end(); i++) {
                if(v[i] == item) {
                    return false;
                } else {
                    if (v[i] < item) {
                        indexToInsert++;
                        break;
                    }
                }
            }
            v.insert(indexToInsert,item);
            return true;*/
        }

        typename vector<Data>::iterator begin() const {
            return v.begin();
        }

        typename vector<Data>::iterator end() const {
            return v.end();
        }
};

#endif
