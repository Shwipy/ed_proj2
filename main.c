#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct{
    char compName[50];
    float compPrice;
} Component;

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

    Component components[15];
    int numComponents;
} Service;

typedef struct ServiceNode {
    Service service;
    struct ServiceNode *next;
} ServiceNode;

typedef struct{
    int id;
    char typeName[30];
    float basePrice;
    char employee[50];

    int cancelled; 

    ServiceNode *running;
    ServiceNode *completed;
} ServiceType;

//Requisito 3b
int countServicesAtMoment(ServiceType st){
    int count = 0;
    ServiceNode * current = st.running;
    while(current != NULL){
        count++;
        current = current->next;

    } 
    return count;

}

//Requisito 3a
void listServicesRunning(ServiceType st){
    ServiceNode * current = st.running;
    while(current != NULL){
        time_t dateNow = time(NULL);
        time_t dateStart = current->service.startDateTimestamp;
        double runTime = difftime(dateNow, dateStart);
        printf("Services running\n ID: %d\n Client: %s\n Equipment: %s\n Time running: %.2f\n", current->service.id, current->service.client, current->service.equipment, runTime);
        current = current->next;
    }
}

//Requisito 3c
void listFinishedServices(ServiceType st){
    ServiceNode * current = st.completed;
    while(current != NULL){
        printf("Services completed\n ID: %d\n Client: %s\n Equipment: %s\n", current->service.id, current->service.client, current->service.equipment);
        current = current->next;
    }
}
//Requisito 3d
float averagePrice(ServiceType st){
    ServiceNode * current = st.completed;
    float sum = 0;
    int countServices = 0;
    while(current != NULL){
        sum += current->service.finalPrice;
        countServices++;
        current = current->next;
    }
    if(countServices == 0)
        return 0;
    else
        return sum/countServices;
}

//Requisito 3e
void durationOfOneService(ServiceType st){
    ServiceNode * current = st.completed;
    if(current == NULL){
        printf("Dont have completed services.\n");
    }else {
            double duration;
            double min;
            double max;
            double sum;
            double avarage;
            int count = 1;

            duration = difftime(current->service.endDateTimestamp, current->service.startDateTimestamp);
            min = duration;
            max = duration;
            sum = duration;

            current = current->next;
        
            while(current != NULL){
                duration = difftime(current->service.endDateTimestamp, current->service.startDateTimestamp);

                if(duration < min){
                    min = duration;
                }
                if(duration > max){
                    max = duration;
                }
                sum += duration;
                count++;
                current = current->next;
        }
        avarage = sum/count;
        printf("The duration of services is:\n The minimum is: %.2f\n The maximum is: %.2f\n The average is: %.2f\n", min, max, avarage);
    }
}

//Requisito 5
void durationOfALLServices(ServiceType database[], int numServiceTypes){
    ServiceNode * current;
    int i;
    double duration;
    double min;
    double max;
    double sum;
    double avarage;
    int count = 0;
    int firstService = 1; //ainda nao encontrei nenhum servico

    for(i = 0; i < numServiceTypes; i++){
        current = database[i].completed;
        
        while(current != NULL){
            duration = difftime(current->service.endDateTimestamp, current->service.startDateTimestamp);
            if(firstService == 1){
                min = duration;
                max = duration;
                sum = duration;
                count = 1;
                firstService = 0; //encontrei um servico
            } else {
                if(duration < min){
                    min = duration;
                }
                if(duration > max){
                    max = duration;
                }
                sum += duration;
                count++;
            }
            current = current->next;
        }
    }
    if(firstService == 1){
        printf("Dont have completed services.\n");
    } else {
        avarage = sum/count;
        printf("The duration of all services is:\n The minimum is: %.2f\n The maximum is: %.2f\n The average is: %.2f\n", min, max, avarage);
    }
}

//----------------------------------------------- Utils ------------------------------------------------

//Cria numero aleatorio para ser o id do tipo de serviço
int createRandomId(ServiceType database[], int numElements){
    int generatingId = 1, id;

    if (numElements == 100){
        printf("Max number of elements reached! :o");
        return 0;
    }

    do{
        generatingId = 0;
        id = (rand() % 100) + 1;

        for (int i = 0; i < numElements; i++){

            if (database[i].id == id){
                generatingId = 1;
                break;
            }
        }

    } while (generatingId);
    return id;
}

//Para verificar se existem Tipos de Serviços
int serviceTypeFilled (int numServiceTypes){
    if (numServiceTypes == 0){
        printf("No Services Types Available\n");
        return 0;
    }
    return 1;
}
//----------------------------------------------- List Things -----------------------------------------------
// Função para visualizar os dados de um tipo de serviço
void printServiceType(ServiceType servType){
    
    printf("[%.3d   %.9s  %.24s  %.2f  %d]", servType.id, servType.typeName, servType.employee, servType.basePrice, servType.cancelled);
    
}

//Função para visualisar todos os tipos de serviços eseus dados
void printServiceTypeList(ServiceType database[], int numServiceTypes){
    if (serviceTypeFilled(numServiceTypes)){
        printf("Code   Type     Employee     Price    Cancelled\n");
        for (int i = 0; i < numServiceTypes; i++){
            printServiceType(database[i]);
            printf("\n");
        }
    }    
}

//----------------------------------------------- Funções -----------------------------------------------
//Adiciona um novo Tipo de Serviço
ServiceType addServiceType(int id){
    ServiceType servType;

    servType.id = id;
    servType.cancelled = 0;
    
    printf("Name of the Service Type:");
    scanf("%s", servType.typeName);
    fflush(stdin);

    printf("Name of the Employee:");
    scanf("%s", servType.employee);
    fflush(stdin);

    printf("Base price of the Service:");
    scanf("%f", &servType.basePrice);
    fflush(stdin);

    //catarinaaa!!! ta null pq estão vazio! :o
    servType.running = NULL;
    servType.completed = NULL;

    return servType;
}

//----------------------------------------------- Menus -----------------------------------------------
//Menu para tratar dos tipos de serviço
int serviceTypeMenu(ServiceType database[], int numServiceTypes){
    int running = 1, option, id, verefyId, searchId;
    ServiceType servType;
    Component comp;

    do{
        printf("\nManage Services:\n");
        printf("1 - Register New Service Type\n");
        printf("2 - List All Service Types\n");
        printf("3 - Edit Service Type\n");
        printf("4 - Finish Service\n");
        printf("0 - Back\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option){
        case 1:
            id = createRandomId(database, numServiceTypes);
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

//----------------------------------------------- Main -----------------------------------------------
//main cycle do coiso
int main(){

    int  option, running =1;

    ServiceType database[100];
    int numServiceTypes = 0;



    do{
        printf("Menu:\n");
        //É preciso mudar esta merda todaaaaa ;-; 
        printf("1 - Manage Service Types\n");
/*      printf("2 - Show [Client] Services\n");
        printf("3 - Lists service by year\n");
        printf("4 - Update service price\n");
        printf("5 - List incomplet services by type\n");
        printf("6 - List complet services by type\n");
        printf("7 - Sorted by client name\n");
        printf("8 - Sorted by end date\n");
        printf("9 - Duration about all services\n");
        printf("10 - Duration about all services by type of service\n");
        printf("11 - Lists all services\n"); // só para demonstração
        printf("12 - Test function CSV\n"); */
        printf("0 - Exit\n");
        printf("What do you want to do:");
        fflush(stdin);
        scanf("%d", &option);

        switch (option){
        case 1:
            numServiceTypes = serviceTypeMenu(database, numServiceTypes);
            printf("%d",numServiceTypes);
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
