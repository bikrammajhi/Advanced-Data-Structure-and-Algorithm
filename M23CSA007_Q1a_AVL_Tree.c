#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key;
    struct node *left;
    struct node *right;
    int height;
};

int height(struct node *N){
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b){
    return (a>b)? a:b;
}

struct node *newNode(int value){
    struct node *pnode = (struct node*)malloc(sizeof(struct node));
    pnode->key = value;
    pnode->left = NULL;
    pnode->right = NULL;
    pnode->height = 1;
    return pnode;
}

int getBalance(struct node *N){
        if(N == NULL)
            return 0;
        else
            return height(N->left) - height(N->right);
}

struct node *minValueSubtree(struct node *N){
    struct node *current = N;
    while(current->left != NULL)
        current = current->left;
    return current;
}

struct node *leftRotate(struct node *x){
    struct node *y = x->right;
    struct node *T2 = y->left;

    y->left = x;
    x->right = T2;
    
    x->height = max(height(x->left),height(x->right)) + 1;
    y->height = max(height(y->left),height(y->right)) + 1;

    return y; 
}

struct node *rightRotate(struct node *x){
    struct node *y = x->left;
    struct node *T2 = y->right;

    y->right = x;
    x->left = T2;
    
    x->height = max(height(x->left),height(x->right)) + 1;
    y->height = max(height(y->left),height(y->right)) + 1;

    return y; 
}

struct node *insertNode(struct node *node, int key){
    if (node == NULL){
        return (newNode(key));
    }
    if (key < node->key)
        node->left = insertNode(node->left, key);
    else if (key > node->key)
        node->right = insertNode(node->right, key);
    else    
        return node;

    node->height = max(height(node->left),height(node->right)) + 1;
    int balance = getBalance(node);

    if(balance > 1 && key < node->left->key) // RR
        return rightRotate(node);
    if(balance < -1 && key > node->right->key) // LL
        return leftRotate(node);

    if(balance > 1 && key > node->left->key) // LR
        node->left = leftRotate(node->left);
        return rightRotate(node);
    if(balance < -1 && key < node->right->key){ // RL
        node->right = rightRotate(node->left);
        return leftRotate(node);
    }

    return node;
}

struct node *deleteNode(struct node *root, int key){
    if (root == NULL)
        return root;
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else{

        if ((root->left == NULL) || (root->right == NULL)){
            struct node *temp = root->left ? root->left : root->right;
            if (temp == NULL){
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;
            free(temp);
        }
        else{
            struct node *temp = minValueSubtree(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }

    }
    if (root == NULL)
        return root;
    root->height = max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Delete Rotation
    if(balance > 1 && getBalance(root->left)>=0)      // R0, R1
        return rightRotate(root); 
    if (balance > 1 && getBalance(root->left) < 0){   // R-1
        root->left = leftRotate(root->left);
        return rightRotate(root);
    } 
    if (balance < -1 && getBalance(root->right) <= 0)   // L0, L-1
        return leftRotate(root);
    if (balance <-1 && getBalance(root->right) > 0){     // L1
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void inOrder(struct node *root){
    if(root!=NULL){
    inOrder(root->left);
    printf("%d ", root->key);
    inOrder(root->right);
    }

}

void postOrder(struct node *root){
    if(root!=NULL){
    postOrder(root->left);
    postOrder(root->right);
    printf("%d ", root->key);
    }

}

void preOrder(struct node *root){
    if(root!=NULL){
    printf("%d ", root->key);
    preOrder(root->left);
    preOrder(root->right);
    
    }

}

struct node* searchNode(struct node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return searchNode(root->left, key);

    else
        searchNode(root->right, key);
}

void deleteTree(struct node **root) {
    if (*root != NULL) {
        deleteTree(&((*root)->left));
        deleteTree(&((*root)->right));
        free(*root);
        *root = NULL; // Set the pointer to NULL after freeing memory
    }
}

int main() {
    struct node *root = NULL;
    int element, choice;

    while (1) {
        printf("\n_________________Options Available______________________\n");
        printf("    1. Insert Node\n");
        printf("    2. Delete Node\n");
        printf("    3. Display\n");
        printf("    4. Search\n");
        printf("    5. Exit\n");

        printf("    Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n\nElement to be inserted into root: ");
                scanf("%d", &element);
                root = insertNode(root, element); // Update root after insertion
                break;
            case 2:
                printf("\n\nEnter the element to be deleted: ");
                scanf("%d", &element);
                root = deleteNode(root, element); // Update root after deletion
                break;
            case 3:
                printf("\n\nPre-Order traversal of the AVL Tree: ");
                preOrder(root);
                printf("\n\nIn-Order traversal of the AVL Tree: ");
                inOrder(root);
                printf("\n\nPost-Order traversal of the AVL Tree: ");
                postOrder(root);
                printf("\n");
                break;
            case 4:
                printf("\n\nEnter the element to be searched: ");
                scanf("%d", &element);
                struct node *result = searchNode(root, element);
                if (result != NULL)
                    printf("Element %d found in the AVL tree.\n", element);
                else
                    printf("Element %d not found in the AVL tree.\n", element);
                break;
            case 5:
                deleteTree(&root);
                return 0;
        }
    }
}