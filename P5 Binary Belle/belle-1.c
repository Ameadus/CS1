/*
Alexander Meade COP3502 Binary Belle.c
*/

#include <stdio.h>
#include <stdlib.h>
#define MAX 100 //no more than 100 directories
#define CHARMAX 20
#define BYTESDIR 17
#include <string.h>

typedef struct filenode {
    int isDir;
    char name[CHARMAX+1];
    struct filenode* left;
    struct filenode* right;
    struct filenode* parent; //pointer to parent
    int numFiles; //count of files in dir either 0,1 or 2
    int numSubDir; //count of nodes in dir either 0,1 or 2 nodes and subdirs samething
    int totalSize; //
} filenode;

void freeTree(filenode* root); //frees the tree
struct filenode* createNode(int fileVal, int subDirVal); //create a node with file/subdir
int subDirCount(filenode* root); //counts number of subdirs aka number of nodes
void deleteNode(struct filenode* root, int value);
int fileCount(filenode* root); //counts number of files aka number of leafs
struct filenode* insertNode(filenode* root, int fileVal, int subDirVal); //inserts either a file/subdir into the tree
int byteSize(filenode* root); //gives the byte size of a file/dif

//helper functions for deletion
int hasOnlyLeftChild(struct filenode *node);
int hasOnlyRightChild(struct filenode *node);
int isLeaf(struct filenode *node);
struct filenode* findNode(struct filenode *current_ptr, int value);
struct filenode* minVal(struct filenode *root);

int main(){
int numInstruct, i;
scanf("%d", &numInstruct);
for(i=0; i< numInstruct; i++){
    filenode* dir = NULL;
    int instType;
    scanf("%d", &instType);

    if(instType == 1)
    {
        char* side,file;
        scanf("%s", side);
        int len = strlen(side);
        scanf("%s", dir->numFiles);
        scanf("%d", dir->totalSize);
        insertNode(dir,dir->numFiles,len);
        continue;
    }

    else if(instType == 2)
    {
        char* side;
        scanf("%s", side);
        int len = strlen(side);
        deleteNode(dir,len);
        continue;
    }

    else if(instType == 3)
    {
        char* side = dir->name;
        scanf("%s", side);
        dir->numFiles = fileCount(dir);
        printf("%d\n", dir->numFiles );
        continue;
    }

    else if(instType == 4)
    {
        char* side = dir->name;
        scanf("%s", side);
        dir->numSubDir = subDirCount(dir);
        printf("%d\n", dir->numSubDir );
        continue;
    }
     else if(instType == 5)
    {
        char* side = dir->name;
        scanf("%s", side);
        dir->totalSize = byteSize(dir);
        printf("%d\n", dir->totalSize );
        continue;
    }
    else{
        //iff not one of the 5 operations
        continue;
    }
}


}

//create a node with file/subdir
struct filenode* createNode(int fileVal, int subDirVal)
{
    struct filenode* temp = malloc(sizeof(struct filenode));
    temp->isDir = 1; //set to true since this is a directory
    temp->numFiles = fileVal; //amount of files either 0,1,2
    temp->numSubDir = subDirVal; //amount of subdirs either 0,1,2
    temp->left = NULL;
    temp->right = NULL;
    temp->totalSize = temp->numFiles + BYTESDIR; //size is the standard size of directory plus how many files are in it

    return temp;
}


struct filenode* insertNode(filenode* root, int fileVal, int subDirVal) {

    // create a node
    if (root == NULL){
        root = createNode(fileVal,subDirVal);
    }
    //insert to right
    if (root->numFiles > root->parent->numFiles && root->numSubDir > root->parent->numSubDir)
    {

      //right subtree
      if (root->right != NULL)
        root->right = insertNode(root->right, fileVal,subDirVal);

      //place node to right of root
      else
      {
        root->right->numFiles = fileVal;
        root->right->numSubDir = subDirVal;
      }
    }

    //insert to left
    else {
      //left subtree
      if (root->left != NULL)
        root->left = insertNode(root->left, fileVal, subDirVal);

      //place node to left of root
      else
        root->left->numFiles = fileVal;
        root->left->numSubDir = subDirVal;
    }
    return root;
}

//frees all values in a tree
void freeTree(filenode* root) {
    if (root != NULL)
    {
        freeTree(root->left); //recursuvely free left
        freeTree(root->right); //rec free right
        free(root);
    }
}

int subDirCount(filenode* root) {
    if (root == NULL) return 0; //empty tree check
        //add number of nodes left and right
        int count = 1 + subDirCount(root->left) + subDirCount(root->right);//rec count number of nodes with kids
        return count;
}

int fileCount(filenode* root){
    if( root == NULL) return 0;//empty tree check
    if (root->left == NULL && root->right == NULL) return 1; //if its a leaf its a file
    else return fileCount(root->left) + fileCount(root->right);//rec count leaf num
}


int byteSize(filenode* root){
  if (root != NULL)
    return subDirCount(root) + fileCount(root);//byte size is the size of subdirs plus the size of files from a given root
  else
    return 0;
}




// Will delete the node storing value in the tree rooted at root.
void deleteNode(struct filenode* root, int value) {

  struct filenode *delnode, *new_del_node, *save_node;
  int save_val;

  delnode = findNode(root, value); //make pointer to node to be deleted

  //leaf node deletion case
  if (isLeaf(delnode)) {

    // Deleting the only node in the tree.
    if (root->parent == NULL) {
      free(root); // free the memory for the node.
      //return NULL;
    }

    //deletes iff left child
    if (value < root->parent->numFiles) {
      free(root->parent->left); //free node
      root->parent->left = NULL;
    }

    // deletes iff right child
    else {
      free(root->parent->right); // Free the memory for the node.
      root->parent->right = NULL;
    }

    //return root; //return root
  }

//only left child case
  if (hasOnlyLeftChild(delnode)) {

    // Deleting the root node of the tree.
    if (root->parent == NULL) {
      save_node = delnode->left;
      free(delnode); // Free the node to delete.
      //return save_node; // Return the new root node of the resulting tree.
    }

    // Deletes the node if it's a left child.
    if (value < root->parent->numFiles) {
      save_node = root->parent->left; // Save the node to delete.
      root->parent->left = root->parent->left->left; // Readjust the parent pointer.
      free(save_node); // Free the memory for the deleted node.
    }

    // Deletes the node if it's a right child.
    else {
      save_node = root->parent->right; // Save the node to delete.
      root->parent->right = root->parent->right->left; // Readjust the parent pointer.
      free(save_node); // Free the memory for the deleted node.
    }

  }

  // case where node has only right kid
  if (hasOnlyRightChild(delnode)) {

    //delete root
    if (root->parent == NULL) {
      save_node = delnode->right;
      free(delnode);
      //return save_node;
    }

    // Delete parent with left child
    if (value < root->parent->numFiles) {
      save_node = root->parent->left;
      root->parent->left = root->parent->left->right;
      free(save_node);
    }

    // Delete parent with right child
    else {
      save_node = root->parent->right;
      root->parent->right = root->parent->right->right;
      free(save_node);
    }
  }

  if(!hasOnlyRightChild(delnode) && !hasOnlyLeftChild(delnode) && !isLeaf(delnode))
  {
      freeTree(delnode);
  }

  //find new node to delete
  new_del_node = minVal(delnode->right);
  save_val = new_del_node->numFiles;

  deleteNode(root, save_val);  //delete the val

  //restore data
  delnode->numFiles = save_val;

}

//helper function for deletion
struct filenode* findNode(struct filenode *current_ptr, int value) {

  //empty tree check
  if (current_ptr != NULL) {

    //find val at root
    if (current_ptr->numFiles == value)
      return current_ptr;

    //search left
    if (value < current_ptr->numFiles)
      return findNode(current_ptr->left, value);

    //search right
    else
      return findNode(current_ptr->right, value);

  }
  else
    return NULL; // No node found.
}

// helper function for deletion returns 1 iff node has only left child
int hasOnlyLeftChild(struct filenode *node) {
  return (node->left!= NULL && node->right == NULL);
}

//helper function for deletion returns 1 iff node has only right child
int hasOnlyRightChild(struct filenode *node) {
  return (node->left== NULL && node->right != NULL);
}

//helper function fo deletion returns 1 iff node is a leaf node
int isLeaf(struct filenode *node) {

  return (node->left == NULL && node->right == NULL);
}

//helper function to find minval used in delete
struct filenode* minVal(struct filenode *root) {

  //root stores minval
  if (root->left == NULL)
    return root;

  //left subtree stores min val
  else
    return minVal(root->left);
}

