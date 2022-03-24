#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        puts("Te faltan los argumentos necesarios para poder continuar el programa.");
        return -1;
    }
    DIR* directorio;
    char ruta_directorio[100] = {'\0'};
    char ruta_archivo[100] = {'\0'};
    char nombre_directorio[100] = {'\0'};
    char nombre_archivo[100] = {'\0'};
    strcpy(nombre_directorio, argv[1]);
    strcpy(nombre_archivo, argv[2]);

    strcat(ruta_directorio, "./");
    strcat(ruta_directorio, nombre_directorio);
    directorio = opendir(nombre_directorio);
    while(directorio)
    {
        puts("Ese directorio ya existe. Por favor escribe otro nombre.");
        strcpy(ruta_directorio, "./");
        scanf("%[^\n]%*c", nombre_directorio);
        strcat(ruta_directorio, nombre_directorio);
        directorio = opendir(nombre_directorio);
    }

    if(mkdir(ruta_directorio, 0777) == 0)
        puts("Creando directorio con todos los permisos");
    else
    {
        puts("Error al crear el directorio");
        return -1;
    }

    int descriptor_archivo;
    strcat(ruta_archivo, "./");
    strcat(ruta_archivo, nombre_directorio);
    strcat(ruta_archivo, "/");
    strcat(ruta_archivo, nombre_archivo);
    strcat(ruta_archivo, ".txt");
    descriptor_archivo = open(ruta_archivo, O_WRONLY);
    while(descriptor_archivo != -1)
    {
        puts("Ese archivo ya existe. Por favor escribe otro nombre.");
        strcpy(ruta_archivo, "./");
        strcat(ruta_archivo, nombre_directorio);
        scanf("%[^\n]%*c", nombre_archivo);
        strcat(ruta_archivo, "/");
        strcat(ruta_archivo, nombre_archivo);
        strcat(ruta_archivo, ".txt");
        descriptor_archivo = open(nombre_archivo, O_WRONLY);
    }
    descriptor_archivo = creat(ruta_archivo, 0777);
    puts("Creando archivo con todos los permisos.");
    if(descriptor_archivo != -1)
        puts("Archivo abierto correctamente.");
    else
        puts("Error al abrir el archivo");

    char buffer_escritura[250] = {'\0'};
    strcpy(buffer_escritura, "Hector Quezada Alanis\n");
    strcat(buffer_escritura, "Jose Manuel Mendez Albarran\n");
    strcat(buffer_escritura, "Esteban Ayala Mayen\n");
    strcat(buffer_escritura, "Octavio de Marcelo Chang\n");
    int bytes_escritos = write(descriptor_archivo, buffer_escritura, strlen(buffer_escritura));

    if(bytes_escritos != -1)
        puts("Escribiendo nombres de equipo.");
    else
    {
        puts("Error al escribir nombres");
        return -1;
    }
    char nombre_enlace[100] = {'\0'};
    strcpy(nombre_enlace, nombre_directorio);
    strcat(nombre_enlace, nombre_archivo);
    strcat(nombre_enlace, "HardLink");

    int enlace = link(ruta_archivo, nombre_enlace);
    if(enlace == 0)
        puts("Enlace fisico creado");
    else
    {
        puts("Error al crear el enlace fisico");
        return -1;
    }
    strcpy(nombre_enlace, nombre_directorio);
    strcat(nombre_enlace, nombre_archivo);
    strcat(nombre_enlace, "SoftLink");

    enlace = symlink(ruta_archivo, nombre_enlace);
    if(enlace == 0)
        puts("Enlace simbolico creado");
    else
    {
        puts("Error al crear el enlace simbolico");
        return -1;
    }
    struct stat stats;

    if(stat(ruta_archivo, &stats) == 0)
        puts("Se logro obtener la informacion del archivo");
    else
    {
        puts("No se pudo obtener la informacion del archivo.");
        return -1;
    }

    printf("Inodo: %ld\n", stats.st_ino);
    printf("Tamaño del archivo en bytes: %ld\n", stats.st_size);
    printf("Numero de enlaces al archivo: %d\n", stats.st_nlink);

    puts("Leyendo toda la información del directorio de trabajo: ");

    DIR *directorio_actual = opendir("./");
    struct dirent *dirInfo;
    while(dirInfo = readdir(directorio_actual))
    {
        printf("Nombre: %s\n", dirInfo->d_name);
        printf("Inodo: %ld\n", dirInfo->d_ino);
    }

    puts("Fin del programa.");
    return 0;
}
