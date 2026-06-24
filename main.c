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
    ServiceNodePointer auxCurrentNode = st.running;
    while (auxCurrentNode != NULL) {
        count++;
        auxCurrentNode = auxCurrentNode->next;
    }
    return count;
}

// Requisito 3a
void listServicesRunning(ServiceType st) {
    ServiceNodePointer auxCurrentNode = st.running;
    while (auxCurrentNode != NULL) {
        time_t dateNow = time(NULL);
        time_t dateStart = auxCurrentNode->service.startDateTimestamp;
        double runTime = difftime(dateNow, dateStart);
        printf("Services running\n ID: %d\n Client: %s\n Equipment: %s\n Time running: %.2f\n", auxCurrentNode->service.id, auxCurrentNode->service.client, auxCurrentNode->service.equipment, runTime);
        auxCurrentNode = auxCurrentNode->next;
    }
}

// Requisito 3c
void listFinishedServices(ServiceType st) {
    ServiceNodePointer auxCurrentNode = st.completed;
    while (auxCurrentNode != NULL) {
        printf("Services completed\n ID: %d\n Client: %s\n Equipment: %s\n",
            auxCurrentNode->service.id, auxCurrentNode->service.client,
            auxCurrentNode->service.equipment);
        auxCurrentNode = auxCurrentNode->next;
    }
}
// Requisito 3d
float averagePrice(ServiceType st) {
    ServiceNodePointer auxCurrentNode = st.completed;
    float sum = 0;
    int countServices = 0;
    while (auxCurrentNode != NULL) {
        sum += auxCurrentNode->service.finalPrice;
        countServices++;
        auxCurrentNode = auxCurrentNode->next;
    }
    if (countServices == 0)
        return 0;
    else
        return sum / countServices;
}

// Requisito 3e
void durationOfOneService(ServiceType st) {
    ServiceNodePointer auxCurrentNode = st.completed;
    if (auxCurrentNode == NULL) {
        printf("Dont have completed services.\n");
    }
    else {
        double duration;
        double min;
        double max;
        double sum;
        double avarage;
        int count = 1;

        duration = difftime(auxCurrentNode->service.endDateTimestamp, auxCurrentNode->service.startDateTimestamp);
        min = duration;
        max = duration;
        sum = duration;

        auxCurrentNode = auxCurrentNode->next;

        while (auxCurrentNode != NULL) {
            duration = difftime(auxCurrentNode->service.endDateTimestamp, auxCurrentNode->service.startDateTimestamp);

            if (duration < min) {
                min = duration;
            }
            if (duration > max) {
                max = duration;
            }
            sum += duration;
            count++;
            auxCurrentNode = auxCurrentNode->next;
        }
        avarage = sum / count;
        printf("The duration of services is:\n The minimum is: %.2f\n The maximum "
            "is: %.2f\n The average is: %.2f\n",
            min, max, avarage);
    }
}
// Requisito 5
void durationOfALLServices(ServiceType database[], int numServiceTypes) {
    ServiceNodePointer auxCurrentNode;
    int i;
    double duration;
    double min;
    double max;
    double sum;
    double avarage;
    int count = 0;
    int firstService = 1; // ainda nao encontrei nenhum servico

    for (i = 0; i < numServiceTypes; i++) {
        auxCurrentNode = database[i].completed;

        while (auxCurrentNode != NULL) {
            duration = difftime(auxCurrentNode->service.endDateTimestamp,
                auxCurrentNode->service.startDateTimestamp);
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
            auxCurrentNode = auxCurrentNode->next;
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
        ServiceNodePointer auxCurrentNode = database[i].running;
        while (auxCurrentNode != NULL) {
            if (auxCurrentNode->service.id == serviceId) {
                existService = 1;
                if (auxCurrentNode->service.numComponents >= 15) {
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
                auxCurrentNode->service.components[auxCurrentNode->service.numComponents] =
                    newComponent;
                auxCurrentNode->service.numComponents++;

                auxCurrentNode->service.finalPrice += newComponent.compPrice;
                printf("The new prioce is: %.2f \n", auxCurrentNode->service.finalPrice);
                return;
            }
            auxCurrentNode = auxCurrentNode->next;
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
        ServiceNodePointer auxCurrentNode = database[i].completed;
        while (auxCurrentNode != NULL) {
            if (auxCurrentNode->service.id == serviceId) {
                if (auxCurrentNode->service.paidFor == 1) {
                    printf("Already paid for.\n");
                }
                else {
                    printf("Total price: %.2f\n Doyou want to pay? (1-yes, 0-noooo)\n", auxCurrentNode->service.finalPrice);
                    int anwser;
                    scanf("%d", &anwser);
                    fflush(stdin);
                    if (anwser == 1) {
                        auxCurrentNode->service.paidFor = 1;
                        auxCurrentNode->service.paymentDate = time(NULL);
                        printf("Success in your payment! :D\n");
                    }
                }return;


            }
            auxCurrentNode = auxCurrentNode->next;
        }
    }
    printf("oh noo... i can't find the service\n");
}

//Requisito 7 com bubble sort
void sortServicesRunning(ServiceType database[], int numServiceTypes) {
    for (int i = 0; i < numServiceTypes - 1; i++) {
        for (int j = i + 1; j < numServiceTypes - 1; j++) {
            if (countServicesAtMoment(database[i]) < countServicesAtMoment(database[j])) {
                ServiceType temp = database[i];
                database[i] = database[j];
                database[j] = temp;
            }

        }
    }
}

//Requisito 4
void sortServicesCompletedByValue(ServiceType database[], int numServiceTypes) {
    for (int i = 0; i < numServiceTypes; i++) {
        ServiceNodePointer auxCurrentNode = database[i].completed;
        while (auxCurrentNode != NULL) {
            ServiceNodePointer j = auxCurrentNode->next;
            while (j != NULL) {
                if (auxCurrentNode->service.finalPrice > j->service.finalPrice) {
                    Service temp = auxCurrentNode->service;
                    auxCurrentNode->service = j->service;
                    j->service = temp;
                }
                j = j->next;
            }
            auxCurrentNode = auxCurrentNode->next;
        }
    }
}

//Requisito 6
void listCancelServices(ServiceType database[], int numServiceTypes) {
    for (int i = 0; i < numServiceTypes; i++) {
        if (database[i].cancelled == 1) {
            int totalServices = countServicesAtMoment(database[i]);
            ServiceNodePointer auxCurrentNode = database[i].completed;
            while (auxCurrentNode != NULL) {
                totalServices++;
                auxCurrentNode = auxCurrentNode->next;
            }
            printf("Cancelled Services\n Total Services: %d\n", totalServices);
        }
    }
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
    ServiceNodePointer auxCurrentNode;

    do {
        generatingId = 0;
        id = (rand() % 100) + 1;

        for (int i = 0; i < numServiceTypes && !generatingId; i++) {

            auxCurrentNode = database[i].running;
            while (auxCurrentNode != NULL) {
                if (auxCurrentNode->service.id == id) {
                    generatingId = 1;
                    break;
                }
                auxCurrentNode = auxCurrentNode->next;
            }

            if (!generatingId) {
                auxCurrentNode = database[i].completed;
                while (auxCurrentNode != NULL) {
                    if (auxCurrentNode->service.id == id) {
                        generatingId = 1;
                        break;
                    }
                    auxCurrentNode = auxCurrentNode->next;
                }
            }
        }

    } while (generatingId);
    return id;
}

//verificar se o tipo existe
int serviceTypeExists(ServiceType database[], int numServiceTypes, int id) {
    for (int i = 0; i < numServiceTypes; i++) {
        if (database[i].id == id) {
            return i;
        }
    }
    return -1;
}

//verificar se o servico em si existe
int serviceExists(ServiceType database[], int numServiceTypes, int id) {
    for (int i = 0; i < numServiceTypes; i++) {
        ServiceNodePointer auxCurrentNode = database[i].running;
        while (auxCurrentNode != NULL) {
            if (auxCurrentNode->service.id == id) {
                return i;
            }
            auxCurrentNode = auxCurrentNode->next;
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

//Limpa um node
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

//Adiciona um novo Servico
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

//Muda um tipo de servico para cancelado
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
    printf("Type was cancelled.\n");
    return 1;
}

//Apaga um tipo se servico e todos os seus nodes da lista (com recursividade)
int deleteServiceType(ServiceType database[], int numServiceTypes, int typeIndex) {

    freeServiceList(database[typeIndex].running);
    freeServiceList(database[typeIndex].completed);

    for (int i = typeIndex; i < numServiceTypes - 1; i++) {
        database[i] = database[i + 1];
    }

    printf("Deleted Everything!!.\n");
    return numServiceTypes - 1;
}

//Adiciona um servico a lista running
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

void finishService(ServiceType database[], int typeIndex, int serviceId) {
    ServiceNodePointer auxCurrentNode = database[typeIndex].running;
    ServiceNodePointer auxPreviousNode = NULL;

    while (auxCurrentNode != NULL) {
        if (auxCurrentNode->service.id == serviceId) {

            auxCurrentNode->service.endDateTimestamp = time(NULL);

            if (auxPreviousNode == NULL) {
                database[typeIndex].running = auxCurrentNode->next;
            }
            else {
                auxPreviousNode->next = auxCurrentNode->next;
            }

            auxCurrentNode->next = database[typeIndex].completed;
            database[typeIndex].completed = auxCurrentNode;

            printf("Service concluded!!\n");
            return;
        }
        auxPreviousNode = auxCurrentNode;
        auxCurrentNode = auxCurrentNode->next;
    }
}

void deleteService(ServiceType database[], int typeIndex, int serviceId) {
    ServiceNodePointer auxCurrentNode = database[typeIndex].running;
    ServiceNodePointer auxPreviousNode = NULL;

    while (auxCurrentNode != NULL) {
        if (auxCurrentNode->service.id == serviceId) {

            if (auxPreviousNode == NULL) {
                database[typeIndex].running = auxCurrentNode->next;
            }
            else {
                auxPreviousNode->next = auxCurrentNode->next;
            }

            free(auxCurrentNode);
            printf("Service deleted!!\n");
            return;
        }
        auxPreviousNode = auxCurrentNode;
        auxCurrentNode = auxCurrentNode->next;
    }
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
            //Para verificar se os tipos foram criados
            printServiceTypeList(database, numServiceTypes);
            break;

        case 3:
            //1b ficou por fazer ;-;
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
            printf("ID of the Service to finish:");
            scanf("%d", &id);
            fflush(stdin);

            typeIndex = serviceExists(database, numServiceTypes, id);
            if (typeIndex == -1) {
                printf("Service Not Found.\n");
                break;
            }

            finishService(database, typeIndex, id);
            break;

        case 4:
            printf("ID of the Service to Delete:");
            scanf("%d", &id);
            fflush(stdin);

            //só fiz para se o servico tiver na em 'running' pq se não tinha de fazer  outra função
            //para ver ser ele estava na lista competed e já não tenho cabeça..
            typeIndex = serviceExists(database, numServiceTypes, id);

            if (typeIndex == -1) {
                printf("Service Not Found or completed.\n");
                break;
            }
            deleteService(database, typeIndex, id);
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
            sortServicesCompletedByValue(database, numServiceTypes);
            break;

        case 5:
            durationOfALLServices(database, numServiceTypes);
            break;

        case 6:
            listCancelServices(database, numServiceTypes);
            break;

        case 7:
            sortServicesRunning(database, numServiceTypes);
            printServiceTypeList(database, numServiceTypes);
            break;

        case 0:
            printf("Turning APP off\n");
            printf("Saving Session Data to Bin(ou não...)");
            /*             writeFiles(prices, database, numServices); */
            return 0;

        default:
            printf("Incorrect option. Try Again!!");
            break;
        }
    } while (running);

    return 0;
}
