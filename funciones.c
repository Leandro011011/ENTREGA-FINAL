#include <stdio.h>
#include <string.h>
#include "funciones.h"

struct Zona zonas[5];
float limitesContaminantes[4] = {400.0, 40.0, 20.0, 25.0};
const char* nombresContaminantes[4] = {"CO2", "NO2", "SO2", "PM2.5"};

int esNumeroPositivo(float num) {
    return num >= 0;
}

void ingresarNombreZona(char* nombre) {
    fgets(nombre, 100, stdin);
    size_t len = strlen(nombre);
    if (len > 0 && nombre[len - 1] == '\n') {
        nombre[len - 1] = '\0';
    }
}



void ingresarDatos() {
    FILE *file = fopen("datos.bin", "ab+");
    if (file == NULL) {
        printf("============================================================\n");
        printf("                  ERROR AL ABRIR ARCHIVO                   \n");
        printf("============================================================\n");
        printf(" No se pudo abrir el archivo de datos. Verifique el sistema.\n");
        printf("============================================================\n");
        return;
    }

    printf("\n============================================================\n");
    printf("     SISTEMA DE MONITOREO Y PREDICCION DE CONTAMINACION  \n");
    printf("============================================================\n");
    printf(" Este programa monitorea la calidad del aire en 5 zonas.\n");
    printf(" Se analizaran los niveles de CO2, SO2, NO2 y PM2.5\n");
    printf(" para evaluar su impacto y predecir valores futuros.\n");
    printf("------------------------------------------------------------\n");
    printf("                NIVELES DE CONTAMINACION ALTOS              \n");
    printf("------------------------------------------------------------\n");
    printf(" - Dioxido de Carbono (CO2)   >= %.2f \n", limitesContaminantes[0]);
    printf(" - Dioxido de Azufre (SO2)    >= %.2f \n", limitesContaminantes[1]);
    printf(" - Dioxido de Nitrogeno (NO2) >= %.2f \n", limitesContaminantes[2]);
    printf(" - Particulas PM2.5           >= %.2f \n", limitesContaminantes[3]);
    printf("============================================================\n\n");

    printf("\n============================================================\n");
    printf("                 INGRESO DE DATOS DE ZONAS                  \n");
    printf("============================================================\n");

    for (int i = 0; i < 5; i++) {
        printf("\n------------------------------------------------------------\n");
        printf("                DATOS DE LA ZONA %d                         \n", i + 1);
        printf("------------------------------------------------------------\n");

        while (getchar() != '\n');  
        printf("Ingrese el nombre de la zona: ");
        ingresarNombreZona(zonas[i].nombre);

        printf("\nIngrese los niveles de contaminantes (CO2, NO2, SO2, PM2.5):\n");

        for (int j = 0; j < 4; j++) {
            do {
                printf("  - %s: ", nombresContaminantes[j]);
                scanf("%f", &zonas[i].contaminantes[j]);
                if (zonas[i].contaminantes[j] < 0) {
                    printf("    Error: Ingrese un numero positivo.\n");
                }
            } while (zonas[i].contaminantes[j] < 0);
        }

        printf("\nIngrese los datos climaticos:\n");

        do {
            printf("  - Temperatura (C): ");
            scanf("%f", &zonas[i].temperatura);
            if (zonas[i].temperatura < 0) {
                printf("    Error: Ingrese un numero positivo.\n");
            }
        } while (zonas[i].temperatura < 0);

        do {
            printf("  - Velocidad del viento (m/s): ");
            scanf("%f", &zonas[i].velocidadViento);
            if (zonas[i].velocidadViento < 0) {
                printf("    Error: Ingrese un numero positivo.\n");
            }
        } while (zonas[i].velocidadViento < 0);

        do {
            printf("  - Humedad (%%): ");
            scanf("%f", &zonas[i].humedad);
            if (zonas[i].humedad < 0) {
                printf("    Error: Ingrese un numero positivo.\n");
            }
        } while (zonas[i].humedad < 0);

        zonas[i].dia = i + 1;
    }

    fwrite(zonas, sizeof(struct Zona), 5, file);
    fclose(file);

    printf("\n============================================================\n");
    printf("         TODOS LOS DATOS HAN SIDO GUARDADOS CORRECTAMENTE   \n");
    printf("============================================================\n");
}


void leerdatosGuardados(struct Zona *zonasGuardadas, int *Regristro) {
    FILE *file = fopen("datos.bin", "rb");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    *Regristro=0;
    while (fread(&zonasGuardadas[*Regristro], sizeof(struct Zona), 1, file))
    {
        (*Regristro)++;   
    }
    ;
    fclose(file);
}
void monitoreoContaminacion() {
    struct Zona zonasguardadas[20];
    int regnum = 0;
    leerdatosGuardados(zonasguardadas, &regnum);

    printf("\n=================================================================\n");
    printf("                 MONITOREO DE CONTAMINACION                      \n");
    printf("=================================================================\n");

    for (int i = 0; i < regnum; i++) {
        printf("\n---------------------------------------------------------------\n");
        printf("  Zona: %s\n", zonasguardadas[i].nombre);
        printf("---------------------------------------------------------------\n");

        for (int j = 0; j < 4; j++) {
            printf("  Nivel de %s: %.2f\n", nombresContaminantes[j], zonasguardadas[i].contaminantes[j]);

            if (zonasguardadas[i].contaminantes[j] > limitesContaminantes[j]) {
                printf("\n  **ADVERTENCIA:** Nivel de %s excede el limite!\n", nombresContaminantes[j]);
                printf("  Medidas recomendadas:\n");
                printf("  - Paralizar la quema de materia prima\n");
                printf("  - Reducir el tiempo de trabajo en industrias\n");
            }
        }
    }

    printf("\n=================================================================\n");
    printf("              FIN DEL MONITOREO DE CONTAMINACION                 \n");
    printf("=================================================================\n");
}

//guardar las preddiciones en un archivo binario
void save (struct Zona *zonasGuardadas, int *Regristro){
    FILE *file = fopen("predicciones.bin", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    fwrite(zonasGuardadas, sizeof(struct Zona), *Regristro, file);
    fclose(file);
}

void predecirContaminantes() {
    struct Zona zonasguardadas[20];
    int regnum = 0;
    leerdatosGuardados(zonasguardadas, &regnum);

    // Pesos para ponderación
    int pesos[5] = {1, 2, 3, 4, 5};

    // Variables para calcular promedios ponderados
    float sumaPonderadaTemp = 0, sumaPonderadaViento = 0, sumaPonderadaHumedad = 0;
    int sumaPesosClima = 0;

    // Cálculo de promedios ponderados de los datos climáticos
    for (int i = 0; i < regnum; i++) {
        for (int k = 0; k < 5; k++) {
            if (i - k >= 0) {
                sumaPonderadaTemp += zonasguardadas[i - k].temperatura * pesos[k];
                sumaPonderadaViento += zonasguardadas[i - k].velocidadViento * pesos[k];
                sumaPonderadaHumedad += zonasguardadas[i - k].humedad * pesos[k];
                sumaPesosClima += pesos[k];
            }
        }
    }

    // Promedios calculados
    float promedioTemp = sumaPonderadaTemp / sumaPesosClima;
    float promedioViento = sumaPonderadaViento / sumaPesosClima;
    float promedioHumedad = sumaPonderadaHumedad / sumaPesosClima;

    // Mostrar predicciones climáticas
    printf("\n============================================================\n");
    printf("                 PREDICCION CLIMATICA                       \n");
    printf("============================================================\n");
    printf("  Temperatura estimada:       %.2f C\n", promedioTemp);
    printf("  Velocidad del viento:       %.2f m/s\n", promedioViento);
    printf("  Humedad estimada:           %.2f %%\n", promedioHumedad);
    printf("============================================================\n");

    save(zonasguardadas, &regnum);

    // Cálculo de predicciones de contaminantes
    float valores[4];

    for (int j = 0; j < 4; j++) {
        float sumaPonderada = 0;
        int sumaPesos = 0;

        for (int i = 0; i < regnum; i++) {
            for (int k = 0; k < 5; k++) {
                if (i - k >= 0) {
                    sumaPonderada += zonasguardadas[i - k].contaminantes[j] * pesos[k];
                    sumaPesos += pesos[k];
                }
            }
        }

        valores[j] = sumaPonderada / sumaPesos;
    }

    // Mostrar predicciones de contaminante
    printf("\n============================================================\n");
    printf("           PREDICCIONES DE NIVELES DE CONTAMINANTES         \n");
    printf("============================================================\n");

     if (valores[0] > limitesContaminantes[0]) {
        printf("\n------------------------------------------------------------\n");
        printf("  ALERTA: Nivel elevado de %s\n", nombresContaminantes[0]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[0], limitesContaminantes[0]);
        printf("  Medidas recomendadas:\n");
        printf("  - Reduccion del trafico vehicular\n");
    } else {
        printf("\n------------------------------------------------------------\n");
        printf("  Nivel de %s dentro del limite\n", nombresContaminantes[0]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[0], limitesContaminantes[0]);
    }

    if (valores[1] > limitesContaminantes[1]) {
        printf("\n------------------------------------------------------------\n");
        printf("  ALERTA: Nivel elevado de %s\n", nombresContaminantes[1]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[1], limitesContaminantes[1]);
        printf("  Medidas recomendadas:\n");
        printf("  - Cierre temporal de industrias\n");
    } else {
        printf("\n------------------------------------------------------------\n");
        printf("  Nivel de %s dentro del limite\n", nombresContaminantes[1]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[1], limitesContaminantes[1]);
    }

    if (valores[2] > limitesContaminantes[2]) {
        printf("\n------------------------------------------------------------\n");
        printf("  ALERTA: Nivel elevado de %s\n", nombresContaminantes[2]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[2], limitesContaminantes[2]);
        printf("  Medidas recomendadas:\n");
        printf("  - Suspension de actividades al aire libre\n");
    } else {
        printf("\n------------------------------------------------------------\n");
        printf("  Nivel de %s dentro del limite\n", nombresContaminantes[2]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[2], limitesContaminantes[2]);
    }

    if (valores[3] > limitesContaminantes[3]) {
        printf("\n------------------------------------------------------------\n");
        printf("  ALERTA: Nivel elevado de %s\n", nombresContaminantes[3]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[3], limitesContaminantes[3]);
        printf("  Medidas recomendadas:\n");
        printf("  - Uso de mascarilla\n");
    } else {
        printf("\n------------------------------------------------------------\n");
        printf("  Nivel de %s dentro del limite\n", nombresContaminantes[3]);
        printf("  Valor registrado: %.2f (Limite: %.2f)\n", valores[3], limitesContaminantes[3]);
    }


    printf("\n============================================================\n");
    printf("         DATOS DE PREDICCION GUARDADOS EXITOSAMENTE         \n");
    printf("============================================================\n");

    save(zonasguardadas, &regnum);
}




void calcularPromediosHistoricos() {
    struct Zona zonasguardadas[20];
    int regnum = 0;
    leerdatosGuardados(zonasguardadas, &regnum);

    printf("\n=================================================================\n");
    printf("                    PROMEDIOS HISTORICOS                         \n");
    printf("=================================================================\n");

    for (int j = 0; j < 4; j++) {
        float promedio = 0;
        for (int i = 0; i < 5; i++) {
            promedio += zonasguardadas[i].contaminantes[j];
        }
        promedio /= regnum;

        printf("\n---------------------------------------------------------------\n");
        printf("  Promedio historico de %s: %.2f\n", nombresContaminantes[j], promedio);
        printf("---------------------------------------------------------------\n");

        if (promedio > limitesContaminantes[j]) {
            printf("    ALERTA: El promedio historico de %s supera el limite!\n", nombresContaminantes[j]);
            printf("  Medidas recomendadas:\n");
            printf("  - Reduccion del trafico vehicular\n");
            printf("  - Cierre temporal de industrias\n");
            printf("  - Suspension de actividades al aire libre\n");
        }
    }
}

void mostrarDatosGuardados() {
    struct Zona zonasguardadas[100];
    int regnum = 0;
    leerdatosGuardados(zonasguardadas, &regnum);

    printf("\n============================================================\n");
    printf("\t\tDATOS GUARDADOS\n");
    printf("============================================================\n");

    for (int i = 0; i < regnum; i++) {
        printf("\n------------------------------------------------------------\n");
        printf("\tZona: %s\n", zonasguardadas[i].nombre);
        printf("------------------------------------------------------------\n");

        for (int j = 0; j < 4; j++) {
            printf("\tNivel de %s:\t%.2f\n", nombresContaminantes[j], zonasguardadas[i].contaminantes[j]);
        }

        printf("\tTemperatura:\t%.2f C\n", zonasguardadas[i].temperatura);
        printf("\tVelocidad del viento:\t%.2f m/s\n", zonasguardadas[i].velocidadViento);
        printf("\tHumedad:\t%.2f%%\n", zonasguardadas[i].humedad);
        printf("------------------------------------------------------------\n");
    }
}


void menu() {
    int opcion;
    do {
        printf("\033[1;34m");
        printf("\n=========================================================\n");
        printf("                     MENU PRINCIPAL                      \n");
        printf("=========================================================\n");
        printf("\033[0m");
        printf(" 1. Ingresar datos del dia actual\n");
        printf(" 2. Monitoreo de contaminacion actual\n");
        printf(" 3. Prediccion de niveles para el dia siguiente\n");
        printf(" 4. Calcular promedios historicos\n");
        printf(" 5. Mostrar datos guardados\n");
        printf(" 6. Salir\n");
        printf("=========================================================\n");
        printf(" Seleccione una opcion (1-6): ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                ingresarDatos();
                break;
            case 2:
                printf("\n  Monitoreando la contaminacion actual...\n");
                monitoreoContaminacion();
                break;
            case 3:
                printf("\n  Calculando predicciones para el dia siguiente...\n");
                predecirContaminantes();
                break;
            case 4:
                printf("\n  Calculando promedios historicos...\n");
                calcularPromediosHistoricos();
                break;
            case 5:
                printf("\n  Mostrando datos guardados...\n");
                mostrarDatosGuardados();
                break;
            case 6:
                printf("\n  Gracias por usar el programa.....\n");
                break;
            default:
                printf("\n  Opcion no valida. Por favor, intente de nuevo.....N/A\n");
        }
        printf("\n=========================================================\n");

    } while (opcion != 6);
}
