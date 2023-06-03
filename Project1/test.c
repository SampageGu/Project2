void handleAction(int action);

int main() {
    int action = 0;
    while (1) {
        printf("choose(1, 2, or 3):\n");
        scanf("%d", &action);

        handleAction(action);
    }

    return 0;
}

void handleAction(int action) {
  
    switch (action) {
    case 1:
        printf("you choose1.\n");
        break;
    case 2:
        printf("you choose2.\n");
        break;
    case 3:
        printf("you choose3.\n");
        exit(0);
    default:
        printf("no choose.\n");
    }
}
