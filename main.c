#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct {
    char compName[50];
    float compPrice;
} Component;

typedef struct {
    int id;
    char client[50];
    char equipment[50];

    time_t startDateTimestamp;
    time_t endDateTimestamp;

    float finalPrice;
    int paidFor;
    time_t paymentDate;

    Component components[15];
    int numComponents;
} Service;

typedef struct ServiceNode* ServiceNodePointer;

typedef struct ServiceNode {
    Service service;
    struct ServiceNode* next;
} ServiceNode;

typedef struct {
    int id;
    char typeName[30];
    float basePrice;
    char employee[50];

    int cancelled;

    ServiceNode* running;
    ServiceNode* completed;
} ServiceType;

// Requisito 3b
int countServicesAtMoment(ServiceType st) {
    int count = 0;
    ServiceNodePointer current = st.running;
    while (current != NULL) {
        count++;
        current = current->next;
    }
    return count;
}

// Requisito 3a
void listServicesRunning(ServiceType st) {
    ServiceNodePointer current = st.running;
    while (current != NULL) {
        time_t dateNow = time(NULL);
        time_t dateStart = current->service.startDateTimestamp;
        double runTime = difftime(dateNow, dateStart);
        printf("Services running\n ID: %d\n Client: %s\n Equipment: %s\n Time running: %.2f\n", current->service.id, current->service.client, current->service.equipment, runTime);
        current = current->next;
    }
}

// Requisito 3c
void listFinishedServices(ServiceType st) {
    ServiceNodePointer current = st.completed;
    while (current != NULL) {
        printf("Services completed\n ID: %d\n Client: %s\n Equipment: %s\n",
            current->service.id, current->service.client,
            current->service.equipment);
        current = current->next;
    }
}
// Requisito 3d
float averagePrice(ServiceType st) {
    ServiceNodePointer current = st.completed;
    float sum = 0;
    int countServices = 0;
    while (current != NULL) {
        sum += current->service.finalPrice;
        countServices++;
        current = current->next;
    }
    if (countServices == 0)
        return 0;
    else
        return sum / countServices;
}

// Requisito 3e
void durationOfOneService(ServiceType st) {
    ServiceNodePointer current = st.completed;
    if (current == NULL) {
        printf("Dont have completed services.\n");
    }
    else {
        double duration;
        double min;
        double max;
        double sum;
        double avarage;
        int count = 1;

        duration = difftime(current->service.endDateTimestamp,
            current->service.startDateTimestamp);
        min = duration;
        max = duration;
        sum = duration;

        current = current->next;

        while (current != NULL) {
            duration = difftime(current->service.endDateTimestamp,
                current->service.startDateTimestamp);

            if (duration < min) {
                min = duration;
            }
            if (duration > max) {
                max = duration;
            }
            sum += duration;
            count++;
            current = current->next;
        }
        avarage = sum / count;
        printf("The duration of services is:\n The minimum is: %.2f\n The maximum "
            "is: %.2f\n The average is: %.2f\n",
            min, max, avarage);
    }
}
// Requisito 5
void durationOfALLServices(ServiceType database[], int numServiceTypes) {
    ServiceNodePointer current;
    int i;
    double duration;
    double min;
    double max;
    double sum;
    double avarage;
    int count = 0;
    int firstService = 1; // ainda nao encontrei nenhum servico

    for (i = 0; i < numServiceTypes; i++) {
        current = database[i].completed;

        while (current != NULL) {
            duration = difftime(current->service.endDateTimestamp,
                current->service.startDateTimestamp);
            if (firstService == 1) {
                min = duration;
                max = duration;
                sum = duration;
                count = 1;
                firstService = 0; // encontrei um servico
            }
            else {
                if (duration < min) {
                    min = duration;
                }
                if (duration > max) {
                    max = duration;
                }
                sum += duration;
                count++;
            }
            current = current->next;
        }
    }
    if (firstService == 1) {
        printf("Dont have completed services.\n");
    }
    else {
        avarage = sum / count;
        printf("The duration of all services is:\n The minimum is: %.2f\n The "
            "maximum is: %.2f\n The average is: %.2f\n",
            min, max, avarage);
    }
}

// Requisito 2d
void addComponent(ServiceType database[], int numServiceTypes) {
    int serviceId;
    int existService = 0;

    if (numServiceTypes == 0) {
        printf("UPSSS.. no service type available :(\n");
        return;
    }
    printf("PLSSS enter the id of the service you want to add componets\n");
    if (scanf("%d", &serviceId) != 1) {
        printf("Upss.. this id is not valid!\n");
        return;
    }
    fflush(stdin);
    for (int i = 0; i < numServiceTypes; i++) {
        ServiceNodePointer current = database[i].running;
        while (current != NULL) {
            if (current->service.id == serviceId) {
                existService = 1;
                if (current->service.numComponents >= 15) {
                    printf("ohh noo.. max of components :(");
                    return;
                }
                Component newComponent;
                printf("Plss enter the name of the component\n");
                scanf("%s", newComponent.compName);
                fflush(stdin);
                printf("Plss enter the price of the component\n");
                scanf("%f", &newComponent.compPrice);
                fflush(stdin);

                // para guardar no sitio certinho!!
                current->service.components[current->service.numComponents] =
                    newComponent;
                current->service.numComponents++;

                current->service.finalPrice += newComponent.compPrice;
                printf("The new prioce is: %.2f \n", current->service.finalPrice);
                return;
            }
            current = current->next;
        }
    }
    if (existService == 0) {
        printf("oh noo... i can't find the service with this id\n");
    }
}

//Requisito 2e
void valuePayedForService(ServiceType database[], int numServiceTypes) {
    int serviceId;
    printf("Plss enter the id of the service you want to pay\n");
    scanf("%d", &serviceId);
    fflush(stdin);

    for (int i = 0; i < numServiceTypes; i++) {
        ServiceNodePointer current = database[i].completed;
        while (current != NULL) {
            if (current->service.id == serviceId) {
                if (current->service.paidFor == 1) {
                    printf("You don't have to pay for this service, this service is already payed.\n");
                }
                else {
                    printf("Total price: %.2f\n Doyou want to pay? (1-yes, 0-noooo)\n", current->service.finalPrice);
                    int anwser;
                    scanf("%d", &anwser);
                    fflush(stdin);
                    if (anwser == 1) {
                        current->service.paidFor = 1;
                        current->service.paymentDate = time(NULL);
                        printf("Success in your payment! :D\n");
                    }
                }return;


            }
            current = current->next;
        }
    }
    printf("oh noo... i can't find the service\n");
}

//----------------------------------------------- Utils

// Cria numero aleatorio para ser o id do tipo de serviço
int createRandomTypeId(ServiceType database[], int numElements) {
    int generatingId = 1, id;

    if (numElements == 100) {
        printf("Max number of elements reached! :o");
        return 0;
    }

    do {
        generatingId = 0;
        id = (rand() % 100) + 1;

        for (int i = 0; i < numElements; i++) {

            if (database[i].id == id) {
                generatingId = 1;
                break;
            }
        }

    } while (generatingId);
    return id;
}

int generateServiceId(ServiceType database[], int numServiceTypes) {
    int generatingId = 1, id;
    ServiceNodePointer auxNode;

    do {
        generatingId = 0;
        id = (rand() % 100) + 1;

        for (int i = 0; i < numServiceTypes && !generatingId; i++) {

            auxNode = database[i].running;
            while (auxNode != NULL) {
                if (auxNode->service.id == id) {
                    generatingId = 1;
                    break;
                }
                auxNode = auxNode->next;
            }

            if (!generatingId) {
                auxNode = database[i].completed;
                while (auxNode != NULL) {
                    if (auxNode->service.id == id) {
                        generatingId = 1;
                        break;
                    }
                    auxNode = auxNode->next;
                }
            }
        }

    } while (generatingId);
    return id;
}

int serviceTypeExists(ServiceType database[], int numServiceTypes, int id) {
    for (int i = 0; i < numServiceTypes; i++) {
        if (database[i].id == id) {
            return i;
        }
    }
    return -1;
}

// Para verificar se existem Tipos de Serviços
int serviceTypeFilled(int numServiceTypes) {
    if (numServiceTypes == 0) {
        printf("No Services Types Available\n");
        return 0;
    }

    return 1;
}

void freeServiceList(ServiceNode* node) {
    if (node == NULL) {
        return;
    }

    //recursividade!!!!!
    freeServiceList(node->next);
    free(node);
}

//----------------------------------------------- Listar Things and Coisas

void printServiceType(ServiceType servType) {

    printf("[%.3d   %.9s  %.24s  %.2f  %d]", servType.id, servType.typeName,
        servType.employee, servType.basePrice, servType.cancelled);
}

// Função para visualisar todos os tipos de serviços eseus dados
void printServiceTypeList(ServiceType database[], int numServiceTypes) {
    if (serviceTypeFilled(numServiceTypes)) {
        printf("Code   Type     Employee     Price    Cancelled\n");
        for (int i = 0; i < numServiceTypes; i++) {
            printServiceType(database[i]);
            printf("\n");
        }
    }
}

//----------------------------------------------- Funções
//Adiciona um novo Tipo de Serviço
ServiceType addServiceType(int id) {
    ServiceType newServType;

    newServType.id = id;
    newServType.cancelled = 0;

    printf("Name of the Service Type:");
    scanf("%s", newServType.typeName);
    fflush(stdin);

    printf("Name of the Employee:");
    scanf("%s", newServType.employee);
    fflush(stdin);

    printf("Base price of the Service:");
    scanf("%f", &newServType.basePrice);
    fflush(stdin);

    // catarinaaa!!! ta null pq estão vazio! :o
    newServType.running = NULL;
    newServType.completed = NULL;

    return newServType;
}

Service addService(int id) {
    Service newService;
    time_t currentTime;

    newService.id = id;

    printf("Name of the Client:");
    scanf("%s", newService.client);
    fflush(stdin);

    printf("Name of the Equipment:");
    scanf("%s", newService.equipment);
    fflush(stdin);

    newService.startDateTimestamp = time(&currentTime);
    newService.endDateTimestamp = 0;

    newService.finalPrice = 0;
    newService.paidFor = 0;
    newService.paymentDate = 0;

    newService.numComponents = 0;

    return newService;
}

int cancelServiceType(ServiceType database[], int numServiceTypes, int id) {
    int typeIndex = serviceTypeExists(database, numServiceTypes, id);

    if (typeIndex == -1) {
        printf("Service Type Not Found.\n");
        return 0;
    }

    if (database[typeIndex].cancelled) {
        printf("This Type is already cancelled.\n");
        return 0;
    }

    database[typeIndex].cancelled = 1;
    printf("Service Type cancelled successfully! No new services of this type can be created.\n");
    return 1;
}

int deleteServiceType(ServiceType database[], int numServiceTypes, int typeIndex) {

    //liberta as duas listas ligadas associadas a este tipo de serviço
    freeServiceList(database[typeIndex].running);
    freeServiceList(database[typeIndex].completed);

    //fecha o "buraco" deixado no array, deslocando os elementos seguintes uma posição para trás
    for (int i = typeIndex; i < numServiceTypes - 1; i++) {
        database[i] = database[i + 1];
    }

    printf("Service Type and all its services were deleted successfully.\n");
    return numServiceTypes - 1;
}

int insertServiceRunning(ServiceType database[], int typeId, Service service) {
    ServiceNode* newNode = malloc(sizeof(ServiceNode));

    if (newNode == NULL) {
        printf("No memory! :C\n");
        return 0;
    }

    newNode->service = service;
    newNode->next = database[typeId].running;
    database[typeId].running = newNode;

    printf("Service created!!\n");
    return 1;
}

//----------------------------------------------- Menus
//Menu para tratar dos tipos de serviço
int serviceTypeMenu(ServiceType database[], int numServiceTypes) {
    int running = 1, option, id, typeIndex, searchId;
    ServiceType servType;
    Component comp;

    do {
        printf("\nManage Services:\n");
        printf("1 - Register New Service Type\n");
        printf("2 - List All Service Types\n");
        printf("3 - Edit Service Type\n");
        printf("4 - Delete Service Type (forever)\n");
        printf("5 - Cancel Service Type\n");
        printf("0 - Back\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option) {
        case 1:

            id = createRandomTypeId(database, numServiceTypes);
            servType = addServiceType(id);
            database[numServiceTypes] = servType;
            numServiceTypes++;
            break;

        case 2:
            printServiceTypeList(database, numServiceTypes);
            break;

        case 3:

            break;

        case 4:
            printf("ID of the Service Type to delete:");
            scanf("%d", &id);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, id);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            printf("Are you sure?(1-Yes / 0-No):");
            scanf("%d", &option);
            fflush(stdin);

            if (option == 1) {
                numServiceTypes = deleteServiceType(database, numServiceTypes, typeIndex);
            }
            else {
                printf("Canceling Delete.\n");
            }

            break;

        case 5:
            printf("ID of the Service Type to cancel:");
            scanf("%d", &id);
            fflush(stdin);

            cancelServiceType(database, numServiceTypes, id);
            break;
            break;

        case 0:
            printf("\n");
            return numServiceTypes;

        default:
            printf("Incorrect option. Try Again");
            break;
        }
    } while (running);
}

// MENU de Servicos
int serviceMenu(ServiceType database[], int numServiceTypes) {
    int running = 1, option, id, typeId, typeIndex, searchId;
    Service serv;
    Component comp;

    do {
        printf("\nManage Services:\n");
        printf("1 - Register New Service \n");
        printf("2 - List Services By Type \n");
        printf("3 - Finish Service \n");
        printf("4 - Delete Service \n");
        printf("5 - Add Component \n");
        printf("6 - Pay for a Service \n");

        printf("0 - Back\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option) {
            // CRIAR E METER NA LISTA LIGADA O SERVICO
        case 1:

            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].cancelled) {
                printf("This Service Type has been cancelled.\n");
                break;
            }

            id = generateServiceId(database, numServiceTypes);
            serv = addService(id);

            if (!insertServiceRunning(database, typeIndex, serv)) {
                printf("Failed to add service.\n");
            }

            break;
        case 2:

            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].cancelled) {
                printf("This Service Type has been cancelled.\n");
                break;
            }

            if (database[typeIndex].running == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                listServicesRunning(database[typeIndex]);
            }
            break;

        case 3:

            break;

        case 4:

            break;

        case 5:
            addComponent(database, numServiceTypes);
            break;

        case 6:
            valuePayedForService(database, numServiceTypes);
            break;

        case 0:
            printf("\n");
            return 0;

        default:
            printf("Incorrect option. Try Again");
            break;
        }
    } while (running);
}

// não sei como lhe chamar ahhhhhhhh!!!!
int alinea3menu(ServiceType database[], int numServiceTypes) {
    int running = 1, option, id, typeId, typeIndex, searchId;
    float price = 0;
    Service serv;
    Component comp;

    do {
        printf("\nManage Services:\n");
        printf("1 - List Ongoing Services \n");
        printf("2 - Number of Ongoing Services \n");
        printf("3 - List Finished Services \n");
        printf("4 - Mean Price of Finished Services \n");
        printf("5 - Time Statistics of Finished Services\n");

        printf("0 - Back\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option) {
        case 1:
            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].running == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                listServicesRunning(database[typeIndex]);
            }
            break;

        case 2:
            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].running == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                printf("Number of ongoing services: %d\n", countServicesAtMoment(database[typeIndex]));

            }
            break;

        case 3:
            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].completed == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                listFinishedServices(database[typeIndex]);
            }
            break;

        case 4:

            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].completed == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                price = averagePrice(database[typeIndex]);
                printf("%.2f", price);
            }
            break;

        case 5:
            printf("ID of the type of Service:");
            scanf("%d", &typeId);
            fflush(stdin);

            typeIndex = serviceTypeExists(database, numServiceTypes, typeId);
            if (typeIndex == -1) {
                printf("Service Type Not Found.\n");
                break;
            }

            if (database[typeIndex].completed == NULL) {
                printf("No Repairs of this Type\n");
            }
            else {
                durationOfOneService(database[typeIndex]);
            }
            break;

        case 0:
            printf("\n");
            return 0;

        default:
            printf("Incorrect option. Try Again");
            break;
        }
    } while (running);
}

//----------------------------------------------- Main
//main cycle do coiso
int main() {

    int option, running = 1;

    ServiceType database[100];
    int numServiceTypes = 0;

    do {
        printf("Menu:\n");
        // É preciso mudar esta merda todaaaaa ;-;
        printf("1 - Manage Service Types\n");
        printf("2 - Manage Services\n");
        printf("3 - Manage Service by Type\n");
        printf("4 - Sort Completed Services by Value \n");
        printf("5 - Time Statistics Related to Completed Services\n");
        printf("6 - List Cancelled Service Types\n");
        printf("7 - Sort Types by Number of Ongoing Services\n");

        printf("0 - Exit\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option) {
        case 1:
            numServiceTypes = serviceTypeMenu(database, numServiceTypes);
            break;

        case 2:
            serviceMenu(database, numServiceTypes);
            break;

        case 3:
            alinea3menu(database, numServiceTypes);
            break;

        case 4:
            // alinea4
            break;

        case 5:
            // alinea5
            break;

        case 6:
            // alinea6
            break;

        case 7:
            // alinea7
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
