#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} Node;

Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void printTree(Node* root, int space) {
    if (root == NULL) {
        space += 10;
        printf("\n");
        int i;
        for (i = 5; i < space; i++) {
            printf(" ");
        }
        printf("n\n");

        return;
    }

    space += 10;

    printTree(root->right, space);

    printf("\n");
    int i;
    for (i = 5; i < space; i++) {
        printf(" ");
    }
    printf("%d\n", root->data);

    printTree(root->left, space);
}

void inOrderTraversal(Node* root) {
    if(root == NULL) {
        // printf("-1 ");
        return;
    }
    inOrderTraversal(root->left);
    printf("%d ", root->data);
    inOrderTraversal(root->right);
}

void preOrderTraversal(Node* root) {
    if(root == NULL) {
        // printf("-1 ");
        return;
    }
    printf("%d ", root->data);
    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}

void postOrderTraversal(Node* root) {
    if(root == NULL) {
        // printf("-1 ");
        return;
    }
    postOrderTraversal(root->left);
    postOrderTraversal(root->right);
    printf("%d ", root->data);
}