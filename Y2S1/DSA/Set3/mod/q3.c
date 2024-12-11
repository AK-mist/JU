#include "Tree.h"
#include <stdio.h>
#include <stdlib.h>

Node* insert(Node* root, int key) {
    if (root == NULL) {
        return createNode(key);
    }

    if (key < root->data) {
        root->left = insert(root->left, key);
    } else if (key > root->data) {
        root->right = insert(root->right, key);
    }

    return root;
}

Node* search(Node* root, int key) {
    if (root == NULL || root->data == key) {
        return root;
    }

    if (key < root->data) {
        return search(root->left, key);
    }

    return search(root->right, key);
}

Node* findInOrderPredecessor(Node* root, Node* node) {
    if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL) {
            node = node->right;
        }
        return node;
    }

    Node* pre = NULL;
    while (root != NULL) {
        if (node->data > root->data) {
            pre = root;
            root = root->right;
        } else if (node->data < root->data) {
            root = root->left;
        } else {
            break;
        }
    }

    return pre;
}

Node* findInOrderSuccessor(Node* root, Node* node) {
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    Node* su = NULL;
    while (root != NULL) {
        if (node->data < root->data) {
            su = root;
            root = root->left;
        } else if (node->data > root->data) {
            root = root->right;
        } else {
            break;
        }
    }

    return su;
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL) {
        return root;
    }

    if (key < root->data) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->data) {
        root->right = deleteNode(root->right, key);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = findInOrderSuccessor(root, root);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

int main() {
    Node* root = NULL;
    int choice, key;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Create Binary Search Tree\n");
        printf("2. Search for a key\n");
        printf("3. Find In-Order Predecessor and Successor\n");
        printf("4. Insert a key\n");
        printf("5. Delete a key\n");
        printf("6. In-order Traversal\n");
        printf("7. Display Tree\n");
        printf("8. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the root key: ");
                scanf("%d", &key);
                root = insert(root, key);
                break;
            case 2:
                printf("Enter the key to search: ");
                scanf("%d", &key);
                Node* nodeKey = search(root, key);
                if (nodeKey != NULL) {
                    printf("Key found in the BST.\n");
                } else {
                    printf("Key not found in the BST.\n");
                }
                break;
            case 3:
                printf("Enter the key: ");
                scanf("%d", &key);
                Node* node = search(root, key);
                if (node == NULL) {
                    printf("Key not found in the BST.\n");
                } else {
                    Node* pre = findInOrderPredecessor(root, node);
                    Node* su = findInOrderSuccessor(root, node);
                    printf("In-Order Predecessor: %d\n", pre ? pre->data : -1);
                    printf("In-Order Successor: %d\n", su ? su->data : -1);
                }
                break;
            case 4:
                printf("Enter the key: ");
                scanf("%d", &key);
                root = insert(root, key);
                printf("Key %d inserted into the BST.\n", key);
                break;
            case 5:
                printf("Enter the key: ");
                scanf("%d", &key);
                root = deleteNode(root, key);
                printf("Key %d deleted from the BST.\n", key);
                break;
            case 6:
                printf("In-order Traversal: ");
                inOrderTraversal(root);
                printf("\n");
                break;
            case 7:
                printTree(root, 0);
                break;
            case 8:
                exit(0);
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }
    }

    return 0;
}






