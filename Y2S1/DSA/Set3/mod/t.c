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

Node* createTree(char* arr, int index, int n) {
    Node* root = NULL;
    if (index < n) {
        root = createNode(arr[index] - '0');
        if((2 * index + 1 < n ) && arr[2 * index + 1] != 'n')
            root->left = createTree(arr, 2 * index + 1, n);
        if((2 * index + 2 < n ) && arr[2 * index + 2] != 'n')
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

void checkIfNull(Node* root) {
    if(root == NULL) {
        printf("Empty Tree");
    }
}

int main() {
    Node* root = NULL;
    int max_size = 1000;
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
                char* A = (char*)malloc(max_size * sizeof(char));
                int* arr = (int*)malloc((n)*sizeof(int));
                int i = 0;
                char ch;

                printf("Enter the level order traversal of the binary tree:\n[use 'n' in place of empty nodes]\n");
                scanf(" %c", &ch);
                while (1) {
                    if (ch == '\n') {
                        break;
                    }
                    if (ch != ' ') {
                        A[i++] = ch;
                    }
                    scanf("%c", &ch);
                }
                n = i;

                root = createTree(A, 0, n);
                break;
            case 2:
                printf("In-order Traversal: ");
                checkIfNull(root);
                inOrderTraversal(root);
                printf("\n");
                break;
            case 3:
                printf("Pre-order Traversal: ");
                checkIfNull(root);
                preOrderTraversal(root);
                printf("\n");
                break;
            case 4:
                printf("Post-order Traversal: ");
                checkIfNull(root);
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
