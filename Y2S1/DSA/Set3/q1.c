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

Node* createTree(int* arr, int index, int n) {
    Node* root = NULL;
    if (index < n) {
        root = createNode(arr[index]);
        if((2 * index + 1 < n ) && arr[2 * index + 1] != -1)
            root->left = createTree(arr, 2 * index + 1, n);
        if((2 * index + 2 < n ) && arr[2 * index + 2] != -1)
            root->right = createTree(arr, 2 * index + 2, n);
    }
    return root;
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

int main() {
    Node* root = NULL;
    int choice, data, n;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Create Binary Tree\n");
        printf("2. In-order Traversal\n");
        printf("3. Pre-order Traversal\n");
        printf("4. Post-order Traversal\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the number of nodes in the binary tree: ");
                scanf("%d", &n);
                if(n <= 0) {
                    printf("Would create empty tree.\n");
                    break;
                }

                int* arr = (int*)malloc((2*n+1)*sizeof(int));
                int j = 0;
                printf("Enter the level order traversal of the binary tree:\n");
                for (int i = 0; i < n;) {
                    scanf("%d", &arr[j]);
                    if(arr[j++] != -1) i++;
                }

                root = createTree(arr, 0, j);
                break;
            case 2:
                printf("In-order Traversal: ");
                inOrderTraversal(root);
                printf("\n");
                break;
            case 3:
                printf("Pre-order Traversal: ");
                preOrderTraversal(root);
                printf("\n");
                break;
            case 4:
                printf("Post-order Traversal: ");
                postOrderTraversal(root);
                printf("\n");
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}
