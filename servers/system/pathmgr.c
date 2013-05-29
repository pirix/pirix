#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pathmgr.h"

typedef struct pid_list {
    int pid;
    struct pid_list* next;
} pid_list;

typedef struct node {
    const char* name;
    struct node* next;
    struct node* child;
    pid_list* pids;
} node;

static node* root = NULL;

/**
 * Find a node by name on the same level as curr. If create is set, a
 * new node will be created, if none is found.
 */
static node* find_next_node(node* curr, char* name, int create) {
    node* prev = NULL;

    while (curr) {
        if (strcmp(curr->name, name)) {
            prev = curr;
            curr = curr->next;
        }
        else return curr;
    }

    if (create && prev != NULL) {
        curr = calloc(sizeof(node), 1);
        curr->name = strdup(name);
        prev->next = curr;
        return curr;
    }
    else return NULL;
}

/**
 * Find a node by name which is a child of curr. If create is set, a
 * new node will be created, if none is found.
 */
static node* find_child_node(node* curr, char* name, int create) {
    if (curr->child) {
        return find_next_node(curr->child, name, create);
    }
    else if (create) {
        node* new = calloc(sizeof(node), 1);
        new->name = strdup(name);
        curr->child = new;
        return new;
    }
    else return NULL;
}

/**
 * Find the node which represents path. If create is set, all missing
 * nodes will be created.
 */
static node* lookup_node(char* path, int create) {
    node* curr = root;

	char* pch;
    char* save;
    pch = strtok_r(path, "/", &save);

    while (pch)  {
        curr = find_child_node(curr, pch, create);
        if (!curr) return NULL;
        pch = strtok_r(NULL, "/", &save);
    }

    return curr;
}

int pathmgr_resolve(char* path) {
    node* curr = root;

	char* pch;
    char* save;
    pch = strtok_r(path, "/", &save);

    int last = 0;

    while (pch)  {
        curr = find_child_node(curr, pch, 0);
        if (!curr) break;
        if (curr->pids) last = curr->pids->pid;
        pch = strtok_r(NULL, "/", &save);
    }

    return last ? last : -1;
}

int pathmgr_attach(char* path, int pid) {
    node* path_node = lookup_node(path, 1);
    if (!path_node) return -1;

    pid_list* curr;
    if (!path_node->pids) {
        path_node->pids = malloc(sizeof(pid_list));
        curr = path_node->pids;
    }
    else {
        curr = path_node->pids;
        while (curr->next) curr = curr->next;
        curr->next = malloc(sizeof(pid_list));
        curr = curr->next;
    }

    curr->pid = pid;
    curr->next = NULL;

    return 0;
}

int pathmgr_detach(char* path, int pid) {
    node* path_node = lookup_node(path, 0);
    if (!path_node) return -1;

    pid_list* curr = path_node->pids;

    if (!curr) return -1;

    if (curr->pid == pid) {
        free(path_node->pids);
        path_node->pids = NULL;
        return 0;
    }

    while (curr) {
        if (curr->next->pid == pid) {
            pid_list* new = curr->next->next;
            free(curr->next);
            curr->next = new;
            return 0;
        }
    }

    return -1;
}

void pathmgr_init() {
    root = calloc(sizeof(node), 1);
    root->name = "";
}
