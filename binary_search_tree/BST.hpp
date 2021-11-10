#ifndef BST_HPP
#define BST_HPP
#include "BSTNode.hpp"
#include "BSTIterator.hpp"
#include <iostream>
using namespace std;

template<typename Data>
class BST {

   protected:

      /** Pointer to the root of this BST, or 0 if the BST is empty */
      BSTNode<Data>* root;

      /** Number of Data items stored in this BST. */
      unsigned int isize;

      /** Height of this BST. */
      unsigned int iheight;

   public:

      /** define iterator as an aliased typename for BSTIterator<Data>. */
      typedef BSTIterator<Data> iterator;

      /** Default constructor.
        * Initialize an empty BST.
        */
      BST() : root(0), isize(0), iheight(0) {  }


      /** Default destructor.
        * Delete every node in this BST.
        */
      virtual ~BST() {
         deleteAll(root);
      }

      /** Given a reference to a Data item, insert a copy of it in this BST.
       *  Return  true if the item was added to this BST
       *  as a result of this call to insert,
       *  false if an item equal to this one was already in this BST.
       *  Note: This function should use only the '<' operator when comparing
       *  Data items. (should not use ==, >, <=, >=)  For the reasoning
       *  behind this, see the assignment writeup.
       *  TODO
       */
      virtual bool insert(const Data& item) {
         if (root == nullptr) {
         	root = new BSTNode<Data>(item);
         	return true;
         } else {
         	return insert_helper(root, item);
         }
      }

  		virtual bool insert_helper(BSTNode<Data>* current, const Data& item) const {
  			if (!(item < current->data) && !(current->data < item)) {
	      		return false;
	      	} 

		  	if (item < current->data) {
		  		if (current->left != nullptr){
		  			return insert_helper(current->left, item);
		  		} else {
		  			current->left = new BSTNode<Data>(item);
		  			return true;
		  		} 
		  	} 
		  	if (current->data < item) {
		  		if (current->right != nullptr){
		  			return insert_helper(current->right, item);
		  		} else {
		  			current->right = new BSTNode<Data>(item);
		  			return true;
		  		}
		  	}
  		}


      /** Find a Data item in the BST.
       *  Return an iterator pointing to the item, or pointing past
       *  the last node in the BST if not found.
       *  Note: This function should use only the '<' operator when comparing
       *  Data items. (should not use ==, >, <=, >=).  For the reasoning
       *  behind this, see the assignment writeup.
       *  TODO
       */
      virtual iterator find(const Data& item) const {
      	if (root == nullptr) {
    			return BST::iterator(nullptr);
    		} else {
    			return find_helper(root, item);
    		}
      }

      virtual iterator find_helper(BSTNode<Data>* current, const Data& item) const {
      	if (!(item < current->data) && !(current->data < item)) {
      		return BST::iterator(current);
      	} 

      	if (item < current->data) {
      		if (current->left != nullptr){
      			return find_helper(current->left, item);
      		} else {
      			return BST::iterator(nullptr);
      		} 
      	} 
      	if (current->data < item) {
      		if (current->right != nullptr){
      			return find_helper(current->right, item);
      		} else {
      			return BST::iterator(nullptr);
      		}
      	}
      }


      /** Return the number of items currently in the BST.
       *  TODO 
       */
      unsigned int size() const {
        if (root == nullptr) {
          return 0;
        } else {

          //size = //1 + size_helper(root->left) + size_helper(root->right);
          return size_helper(root);
        }
      }

      unsigned int size_helper(BSTNode<Data>* current) const {
        if (current == nullptr) {
          return 0;
        }

        return size_helper(current->left) + size_helper(current->right) + 1;
        /*int size = 0;
        if (current->left != nullptr) {
          size++;
          size_helper(current->left);
          return size;
        } else {
          return 0;
        }
        if (current->right != nullptr) {
          size++;
          size_helper(current->right);
          return size;
        } else {
          return 0;
        }*/
      }

      /** Return the height of the BST.
       * The height of an empty tree is -1 and the height of a tree
       * with only one node is 0.
       *  TODO  
       */
      unsigned int height() const {
        int height = 0;
        if (root == nullptr) {
          return -1;
        } else {
          return height_helper(root) - 1;
        }
      }

      unsigned int height_helper(BSTNode<Data>* current) const {
        if (current == nullptr) {
          return 0;
        }

        return 1 + max(height_helper(current->left), height_helper(current->left));
      }
      /*unsigned int height() const {
         int height = 0;
         if (root == nullptr) {
            height = -1;
            return height;
         } else {
            height = 1 + height_helper(root->left) + height_helper(root->right);
            return height;
         }
      }

      unsigned int height_helper(BSTNode<Data>* current) const {
        int leftHeight = 0;
        int rightHeight = 0;
        int totHeight = 0;
        if (current->left != nullptr) {
          leftHeight++;
          size_helper(current->left);
        } else {
          return leftHeight = leftHeight + 0;
        }
        if (current->right != nullptr) {
          rightHeight++;
          size_helper(current->right);
        } else {
          return rightHeight = rightHeight + 0;;
        }
        if (leftHeight < rightHeight) {
          totHeight = leftHeight;
        } else {
          totHeight = rightHeight;
        }
        return totHeight;
      }*/

      /** Return true if the BST is empty, else false.
       */
      bool empty() const {
         return isize == 0;
      }

      /** Return an iterator pointing to the first item in the BST (not the root).
       */
      iterator begin() const {
         return BST::iterator(first(root));
      }

      /** Return an iterator pointing past the last item in the BST.
       */
      iterator end() const {
         return typename BST<Data>::iterator(0);
      }


      /** Inorder traverse BST, print out the data of each node in ascending order.
       * Implementing inorder and deleteAll base on the pseudo code is an easy way to get started.
       * Pseudo Code:
       * if current node is null: return;
       * recursively traverse left sub-tree
       * print current node data
       * recursively traverse right sub-tree
       * TODO
       */
      void inorder() const {
        if (root == nullptr) {
          return;
        } else {
          return inorder_helper(root);
        }
      }

      void inorder_helper(BSTNode<Data>* current) const {
        if (current == nullptr) {
          return;
        } else {
          inorder_helper(current->left);
          cout << current->data << " ";
          inorder_helper(current->right);
        }
      }

   private:

      /** Find the first element of the BST
       *  TODO 
       */ 
      static BSTNode<Data>* first(BSTNode<Data>* root) {
          if (root == nullptr) {
            return 0;
          } else {
            return goLeft(root);
          }
      }

      static BSTNode<Data>* goLeft(BSTNode<Data>* current) {
        if (current->left != nullptr) {
          goLeft(current->left);
        } else {
          return current;
        }
      }

      /** do a postorder traversal, deleting nodes
       *   TODO 	
       */
      static void deleteAll(BSTNode<Data>* n) {
         /* Pseudo Code:
            if current node is null: return;
            recursively delete left sub-tree
            recursively delete right sub-tree
            delete current node
            */
        if (n == nullptr) {
          return;
        } else {
          return deleteAll_helper(n);
        }
      }

      static void deleteAll_helper(BSTNode<Data>* current) {
         if (current == nullptr) {
          return;
        } else {
          deleteAll_helper(current->left);
          deleteAll_helper(current->right);
          free(current);
        }
      }
};


#endif //BST_HPP
