#include <stdio.h>
#include <stdlib.h>
#include "Tree.h"

int findIndex(int arr[], int str, int end, int val) {
    int i;
    for (i = str; i <= end; i++) {
        if (arr[i] == val) {
            return i;
        }
    }
    return -1;
}

Node* buildTree(int* preOrder, int* inOrder, int inStr, int inEnd, int* preIdx) {
    if (inStr > inEnd) {
        return NULL;
    }

    Node* newNode = createNode(preOrder[*preIdx]);

    (*preIdx)++;

    if (inStr == inEnd) {
        return newNode;
    }

    int inIdx = findIndex(inOrder, inStr, inEnd, newNode->data);

    newNode->left = buildTree(preOrder, inOrder, inStr, inIdx - 1, preIdx);
    newNode->right = buildTree(preOrder, inOrder, inIdx + 1, inEnd, preIdx);

    return newNode;
}

int main() {
    Node* root = NULL;
    int choice, n, i;

    while(1) {
        printf("\nMenu:\n");
        printf("1. Create Binary Tree\n");
        printf("2. Display Binary Tree\n");
        printf("3. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the number of nodes in the binary tree: ");
                scanf("%d", &n);
                if(n <= 0) {
                    printf("Would create empty tree.\n");
                    break;
                }

                int* preOrder = (int*)malloc(n * sizeof(int));
                int* inOrder = (int*)malloc(n * sizeof(int));
                printf("Enter Pre-Order Traversal:\n");
                for(i=0; i<n; i++) {
                    scanf("%d", &preOrder[i]);
                }
                printf("Enter In-Order Traversal:\n");
                for(i=0; i<n; i++) {
                    scanf("%d", &inOrder[i]);
                }
                int preIdx = 0;
                root = buildTree(preOrder, inOrder, 0, n - 1, &preIdx);
                break;
            case 2:
                printf("Binary Tree: \n");
                printTree(root, 0);
                printf("\n");
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    return 0;
}
