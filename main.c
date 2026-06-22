#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct ServiceNode {
    Service service;
    struct ServiceNode *next;
} ServiceNode;

typedef struct{
    int id;
    char client[50];
    char equipment[50];
    char equipmentCode[20];

    time_t startDateTimestamp;
    time_t endDateTimestamp;

    float finalPrice;
    int payedFor;
    time_t paymentDate;
    int numComponents;
} Service;


typedef struct{
    char typeCode[20];
    char typeName[30];
    float price;
    char employee[50];

    int cancelled; 

    ServiceNode *running;
    ServiceNode *completed;
} ServiceType;


int main(){

    int  option, running =1;

    ServiceType database[100];
    int numServiceTypes;



    do{
        printf("Menu:\n");
        //É preciso mudar esta merda todaaaaa ;-; 
        printf("1 - Manage Services\n");
        printf("2 - Show [Client] Services\n");
        printf("3 - Lists service by year\n");
        printf("4 - Update service price\n");
        printf("5 - List incomplet services by type\n");
        printf("6 - List complet services by type\n");
        printf("7 - Sorted by client name\n");
        printf("8 - Sorted by end date\n");
        printf("9 - Duration about all services\n");
        printf("10 - Duration about all services by type of service\n");
        printf("11 - Lists all services\n"); // só para demonstração
        printf("12 - Test function CSV\n");
        printf("0 - Exit\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option){
        case 1:

            break;

        case 2:

            break;

        case 3:

            break;

        case 0:
            printf("Turning APP off\n");
            printf("Saving Session Data to Bin");
/*             writeFiles(prices, database, numServices); */
            return 0;

        default:
            printf("Incorrect option. Try Again!!");
            break;
        }
    } while (running);

    return 0;
}
