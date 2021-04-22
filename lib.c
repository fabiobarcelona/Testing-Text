#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"lib.h"

/**
*
* Función para comprobar qué comando ha introducido el usuario
*
**/
void checkCommand(int argc, char* argv[])
{
	int compResult; //Variable para determinar el resultado de una comparación de cadenas

	//Si solo se introduce el nombre del programa, solo hay posibilidad para un comando, el que muestra la ayuda de sintaxis
	if (argc == 1)
	{
		cSyntax();
	}

	//Si solo se introducen un argumento , solo hay posibilidad para un comando, el que muestra los datos del autor
	else if (argc == 2)
	{
		//Pasamos el texto del usuario a minúsculas y lo comparamos con "info"
		strlwr(argv[1]);
		compResult = strcmp("info", argv[1]);

		if (compResult != 0)
		{
			printf("El comando introducido no existe"); //Si ha escrito algo distinto, imprimimos error y salimos del programa
			printf("\nFIN");
			exit(EXIT_FAILURE);
		}
		else
		{
			cInfo(); //Si el comando es correcto, llamamos a la función que imprime los datos
		}
	}

	//Si el comando tiene más de un argumento...
	else
	{
		int i = 0;

		//Array que contiene todos los posibles comandos

		char* commands[] = { "cuenta", "busca", "numeros", "fechas", "remplazar", "palabras", "extracto" };
		size_t arraySize = sizeof(commands) / sizeof(commands[0]); //Número de elementos en el array de comandos
		strlwr(argv[2]);

		//Recorremos el array de comandos, comparando la entrada del usuario con cada comando disponible
		//Hasta que demos con el comando correcto o hayamos llegado al final y no haya coincidido ninguno
		do
		{
			//Estructura de control para evitar bucles infinitos
			if (i >= arraySize)
			{
				printf("\nNo se ha encontrado ningun comando\n");
				printf("\nFIN");
				exit(EXIT_FAILURE);
			}

			compResult = strcmp(commands[i], argv[2]);
			i++;

		} while (compResult != 0);

		if (i == 1)
		{
			cCountWord(argc, argv);
		}
		else if (i == 2)
		{
            cSearchWord(argc, argv);
		}
		else if (i == 3)
		{
			cNumbers(argc, argv);
		}
		else if (i == 4)
		{
			cDates(argc, argv);
		}
		else if (i == 5)
		{
			cReplaceWord(argc, argv);
		}
		else if (i == 6)
		{
			cCountWordsExam(argc, argv);
		}
		else if (i == 7)
		{
			cExtract(argc, argv);
		}
	}
}



////////////////////////////////////////////////////
////////////////Funciones Extra//////////////////
////////////////////////////////////////////////////


/**
*
* Función para extraer un número dado de líneas del fichero de lectura en otro fichero de escritura (fichero1 debe existir y num >= 1)
*
* Sintaxis: prog fichero1 extracto num fichero2
*
**/
void cExtract(int argc, char* argv[])
{
	if(atoi(argv[3]) < 1)
	{
		printf("\nEl parametro num debe ser >=1\n");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	FILE* fpHost, *fpGuest;
	int maxLongLine = getLongestLine(argv[1]);
	int fileLines = getFileLines(argv[1]);
	int contLines = 0;
	char* read;

	if(fileLines < atoi(argv[3]))
	{
		printf("\nNo puedes extraer mas lineas de las que tiene el fichero\n");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	fpHost = fopen(argv[1], "r"); //Fichero donde buscamos las líneas
	fpGuest = fopen(argv[4], "w"); //Fichero donde escribimos el resultado

	if(fpHost == NULL)
	{
		perror("Error al abrir fichero host");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	if(fpGuest == NULL)
	{
		perror("Error al abrir fichero guest");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	read = malloc((maxLongLine + 1) * sizeof(char));
	if (read == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	do
	{
		fgets(read, maxLongLine + 2, fpHost);
		fputs(read, fpGuest);
		contLines++;
	}
	while(contLines < atoi(argv[3]));

	free(read);
	fclose(fpHost);
	fclose(fpGuest);
}

/**
*
* Función para contar palabras de un fichero que no sean ni números ni fechas
*
* Sintaxis: prog fichero palabras
*
**/
void cCountWordsExam(int argc, char* argv[])
{
	int i = 0;
	int maxLongLine = 0;
	int contPalabras = 0;
	char delim[] = " \t\n,.;:()[]";
	char* token = NULL;
	char* line;
	FILE* fp;
	
	maxLongLine = getLongestLine(argv[1]);
	//printf("Longitud máxima de linea: %i", maxLongLine);

	fp = fopen(argv[1], "r");

	//Reservamos memoria para la variable que almacena una línea completa
	line = malloc((maxLongLine + 1) * sizeof(char));
	if (line == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	//Tokenizamos el texto del fichero
	while (fgets(line, maxLongLine + 1, fp))
	{
		//printf("\nLinea completa: %s\n", line);
		token = strtok(line, delim);
		while (token != NULL)
		{
			//printf("Token: %s\n", token);
			for(i = 0; i <= strlen(token); i++)
			{
				if(isdigit(token[i]) > 0)
				{
					break;
				}
				else if(isalpha(token[i]) == 0)
				{
					break;
				}
				
				if(i == strlen(token) - 1)
				{
					contPalabras++;
				}
			}
			token = strtok(NULL, delim);
		}
	}

	printf("\n\nRESULTADO DE LA BUSQUEDA\n--------------------------------------------------\n");
	printf("Contador Palabras: %i\n", contPalabras);

	free(line);
	fclose(fp);
}



////////////////////////////////////////////////////
////////////////Comandos Principales////////////////
////////////////////////////////////////////////////



/**
*
* Función para imprimir la sintaxis del programa
*
* Sintaxis: prog
*
**/
void cSyntax()
{
	printf("\n------------------------------------------------------\n");
	printf("prog\n");
	printf("Va sin parámetros (solo el nombre del programa ejecutable). Escribe en pantalla la sintaxis de todos los comandos disponibles a modo de ayuda para el usuario.\n");
	printf("\nprog info\n");
	printf("Muestra por pantalla la siguiente información personal del alumno:\n- Nombre y apellidos\n- email\n");
	printf("\nprog fichero cuenta palabra1 [palabra2] [palabra3] [palabra4]\n");
	printf("Abre el fichero de texto indicado y cuenta cuantas veces se repita cada una de las palabras de la lista en dicho fichero. Al menos ha de indicarse una palabra (palabra1), de manera opcional se podrán indicar más palabras si se desea.\n");
	printf("\nprog fichero busca fichero2\n");
	printf("Análogo al anterior, salvo que ahora las palabras que hay que contar se encuentran en el fichero2, que también habrá que abrir leer para ver cuales son, dichas palabras estarán separadas por los mismos separadores que los tokens del primer fichero. Si el fichero2 no se pudiera abrir también se indicará con un mensaje de error y no se hará nada más. También sería un error que fichero2 estuviera vacío (que no tuviera ninguna palabra).\n");
	printf("\nprog fichero numeros [sum]\n");
	printf("Cuenta cuántos de los tokens del fichero son números, positivos o negativos, enteros o reales con decimales. Se puede indicar el parámetro opcional [sum] si se quiere mostrar la suma total de los números encontrados. IMPORTANTE, el carácter para indicar los decimales es el punto (.)\n");
	printf("\nprog fichero fechas\n");
	printf("Cuenta cuántos de los tokens del fichero son fechas correctas en el formato DD/MM/AAAA (2 dígitos para el día, 2 para el mes y 4 para el año).\n");
	printf("\nprog fichero remplazar palabra1 palabra2\n");
	printf("Sustituye, en el fichero leído, la palabra1 por la palabra2. El resultado se guardará en un fichero nuevo cuyo nombre estará basado en el primero. Por ejemplo, si el fichero se llama “mi_texto.txt”, el nuevo fichero deberá llamarse “mi_texto_new.txt”, en caso de que ya existe un fichero con ese nombre, el nuevo fichero deberá llamarse “mi_texto_new(2).txt” (o “mi_texto_new(3).txt”, etc...). El programa responderá (además de creando el nuevo fichero) mostrando por pantalla la cantidad de veces que ha reemplazado ‘palabra1’ por ‘palabra2’.\n");
	printf("\nprog fichero palabras\n");
	printf("Función para contar palabras de un fichero que no sean ni números ni fechas.\n");
	printf("\nprog fichero1 extracto num fichero2\n");
	printf("Función para extraer un número dado de líneas del fichero de lectura en otro fichero de escritura (fichero1 debe existir y num >= 1).\n");
}

/**
*
* Función para imprimir los datos del autor
*
* Sintaxis: prog info
*
**/
void cInfo()
{
	printf("\n------------------------------------------------------\n");
	printf("\nNombre del Autor: Fabio Barcelona\n");
	printf("\n------------------------------------------------------\n");
}

/**
*
* Función para contar palabras pasadas por linea de comandos en un fichero
*
* Sintaxis: prog fichero cuenta palabra1 ... palabraN
*
**/
void cCountWord(int argc, char* argv[])
{
	int i = 0;
	int maxLongLine = 0;
	int* contOcurrencias;
	char delim[] = " \t\n,.;:()[]";
	char* token = NULL;
	char* line;
	FILE* fp;
	
	maxLongLine = getLongestLine(argv[1]);

	contOcurrencias = malloc((argc - 3) * sizeof(int));
	if (contOcurrencias == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	//Inicializamos el array y extraemos todas las palabras que el usuario quiere buscar
	for (i = 0; i < argc - 3; i++)
	{
		contOcurrencias[i] = 0;
	}

	fp = fopen(argv[1], "r");

	//Reservamos memoria para la variable que almacena una línea completa
	line = malloc((maxLongLine + 1) * sizeof(char));
	if (line == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	//Tokenizamos el texto del fichero
	while (fgets(line, maxLongLine + 1, fp))
	{
		token = strtok(line, delim);
		while (token != NULL)
		{
			for (i = 0; i < argc - 3; i++)
			{
				if (strcmp(token, argv[i + 3]) == 0)
				{
					contOcurrencias[i] += 1;
				}
			}

			token = strtok(NULL, delim);
		}
	}

	printf("\n\nRESULTADO DE LA BUSQUEDA\n--------------------------------------------------\n");
	for (i = 0; i < argc - 3; ++i)
	{
		printf("Palabra: %s\tOcurrencias: %d\n", argv[i + 3], contOcurrencias[i]);
	}

	free(contOcurrencias);
	free(line);
	fclose(fp);
}

/**
*
* Función para contar palabras pasadas por fichero en otro fichero
*
* Sintaxis: prog fichero busca fichero2
*
**/
void cSearchWord(int argc, char* argv[])
{
    int maxLongLineHost = 0;
    int maxLongLineGuest = 0;
	char delim[] = " \t\n,.;:()[]";
	char* token = NULL;
	char* nextTokenGuest = NULL;
    char* line;
    FILE* fpGuest;

    maxLongLineHost = getLongestLine(argv[1]); //Fichero Host (en el que buscamos palabras)
    maxLongLineGuest = getLongestLine(argv[3]); //Fichero Guest (donde se encuentran las palabras a buscar)

	//Tokenizamos el texto del fichero Guest y lo guardamos en un array
	//Para compararlo posteriormente con las palabras del fichero Host
	fpGuest = fopen(argv[3], "r");

	fseek(fpGuest, 0, SEEK_END); //Recorremos el fichero de principio a fin
	
	if(ftell(fpGuest) == 0)
	{
		printf("\nEl fichero2 está vacío\n");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	rewind(fpGuest); //Rebobinar fichero
	
	line = malloc((maxLongLineGuest + 1) * sizeof(char));
	if (line == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}
	
	printf("\n\nRESULTADO DE LA BUSQUEDA\n--------------------------------------------------\n");

	while (fgets(line, maxLongLineGuest + 1, fpGuest))
	{

		//Usamos strtok_r porque necesitamos tokenizar varias cadenas a la vez
		//strtok_r nos permite guardar la última posición para cada llamada.
		token = strtok_r(line, delim, &nextTokenGuest);

		while (token != NULL)
		{
			tokenGuestToHost(token, argv[1], maxLongLineHost);
			token = strtok_r(NULL, delim, &nextTokenGuest);
		}
	}

	free(line);
	fclose(fpGuest);
}

/**
*
* Función para contar números enteros o reales dentro de un fichero
*
* Sintaxis: prog fichero numeros
*
**/
void cNumbers(int argc, char* argv[])
{
	int maxLongLine = 0;
	int contNum = 0;
	float token_float = 0;
	float sumaTotal = 0;
	short sumaTrigger = 0;
	char delimNums[] = " \t\n,;:()[]";
	char* token = NULL;
	char* line;
	FILE* fp;

	if(argv[3])
	{
		strlwr(argv[3]);
		if(strcmp(argv[3], "sum") == 0)
		{
			sumaTrigger = 1;
		}	
	}

	maxLongLine = getLongestLine(argv[1]);

	fp = fopen(argv[1], "r");

	//Tokenizamos el texto del fichero
	line = malloc((maxLongLine + 1) * sizeof(char));
	if (line == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	printf("\n\nRESULTADO DE LA BUSQUEDA\n--------------------------------------------------\n\n");

	while (fgets(line, maxLongLine + 1, fp))
	{
		token = strtok(line, delimNums);
		while (token != NULL)
		{
			contNum += isNum(token);

			if(isNum(token) == 1 && sumaTrigger == 1)
			{
				token_float = atof(token);
				sumaTotal += token_float;
				token_float = 0;
			}
			token = strtok(NULL, delimNums);
		}
	}

	printf("\nNumeros Totales: %i\n", contNum);
	if(sumaTrigger == 1)
	{
		printf("\nSumatorio Total: %.3f\n", sumaTotal);
	}
	free(line);
	fclose(fp);
}

/**
*
* Función para contar cuántas fechas hay en un fichero.
* Formato: DD/MM/AAAA (2 dígitos para el día, 2 para el mes y 4 para el año).
*
* Sintaxis: prog fichero fechas
*
**/
void cDates(int argc, char* argv[])
{
	int maxLongLine = 0;
	int contDates = 0;
	char delim[] = " \t\n,.;:()[]";
	char* token = NULL;
	char* nextTokenLine = NULL;
	char* line;
	FILE* fp;

	maxLongLine = getLongestLine(argv[1]);

	fp = fopen(argv[1], "r");

	line = malloc((maxLongLine + 1) * sizeof(char));
	if (line == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	printf("\n\nRESULTADO DE LA BUSQUEDA\n--------------------------------------------------\n");

	//Tokenizamos el texto del fichero
	while (fgets(line, maxLongLine + 1, fp))
	{
		token = strtok_r(line, delim, &nextTokenLine);
		while (token != NULL)
		{
			contDates += isDate(token);
			
			token = strtok_r(NULL, delim, &nextTokenLine);
		}
	}

	printf("\nFechas Totales: %i\n", contDates);
	free(line);
	fclose(fp);
}


/**
*
* Función para contar remplazar palabra1 por palabra2 dentro del fichero proporcionado
* Este comando genera un nuevo fichero con el resultado y además imprime por pantalla
* El número de veces que se ha reemplazado la palabra.
*
* Sintaxis: prog fichero remplazar palabra1 palabra2
*
**/
void cReplaceWord(int argc, char* argv[])
{
	FILE* fpHost, *fpGuest;
	int contReplaces = 0;
	int maxLongLine = getLongestLine(argv[1]);
	char* read;

	fpHost = fopen(argv[1], "r"); //Fichero donde buscamos la palabra

	char* fileName = getNewFileName(argv[1]);
	fpGuest = fopen(fileName, "w"); //Fichero donde escribimos el resultado

	if(fpHost == NULL)
	{
		perror("Error al abrir fichero host");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	if(fpGuest == NULL)
	{
		perror("Error al abrir fichero guest");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	read = malloc((maxLongLine + 1) * sizeof(char));
	if (read == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	while((fgets(read, maxLongLine + 2, fpHost)) != NULL)
	{
		contReplaces += replaceAll(read, argv[3], argv[4], maxLongLine);
		fputs(read, fpGuest);
	}

	printf("\n\nRESULTADO DE LA EJECUCION\n--------------------------------------------------\n");
	printf("Fichero Generado: %s\n", fileName);
	printf("\nPalabras Reemplazadas: %i", contReplaces);

	fclose(fpHost);
	fclose(fpGuest);
	free(read);
}



////////////////////////////////////////////////////
////////////////Funciones Auxiliares////////////////
////////////////////////////////////////////////////



/**
*
* Función para determinar la línea más larga de un fichero
*
* Usada en: todos los comandos
*
**/
int getLongestLine(char fileDir[])
{
	char c;
	int contador = 0;
	int i = 0;
	int mayor = 0;
	int numFilas = getFileLines(fileDir);
    FILE* fp = fopen(fileDir, "r");

	while (feof(fp) == 0)
	{
		c = fgetc(fp);

		contador++;

		if (c == '\n' || c == EOF)
		{
			if (mayor < contador)
			{
				mayor = contador;
			}
			contador = 0;
			i++;
		}
	}
	
	fclose(fp);
	return mayor;
}

/**
*
* Función para determinar las líneas que tiene un fichero
*
* Usada en: todos los comandos
*
**/
int getFileLines(char fileDir[])
{
	FILE* fp;
	int count_lines = 0;
	char chr;

	fp = fopen(fileDir, "r");

	if (fp == NULL)
	{
		perror("\nError al abrir archivo");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

	//Extraemos el carácter actual y lo almacenamos en chr
	chr = getc(fp);

	while (chr != EOF)
	{
		//Contamos +1 cada salto de línea
		if (chr == '\n')
		{
			count_lines += 1;
		}
		//Cogemos el siguiente carácter del fichero
		chr = getc(fp);
		//Si llegamos al final del fichero, contamos +1 y salimos del bucle
		if (chr == EOF)
		{
			count_lines += 1;
			break;
		}
	}

	fclose(fp); //Cerramos fichero
	return count_lines;
}

/**
*
* Función para comparar una palabra que se pasa por argumento con otra que está localizada en un fichero
* Se le debe pasar como argumento el propio token a comparar, la dirección del fichero que abrir y el tamaño
* Máximo de linea que tendrá el fichero para poder reservar la memoria exacta.
*
* Usada en: busca
*
**/
void tokenGuestToHost(char *tokenGuest, char fileHost[], int maxLine)
{
	int contOcurrencias = 0;
	FILE* f = fopen(fileHost, "r");
	char* tokenHost = NULL;
	char* nextTokenHost = NULL;
	char delim[] = " \t\n,.;:()[]";
	char* lineHost = malloc((maxLine + 1) * sizeof(char));

	if(lineHost == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}
	
	while (fgets(lineHost, maxLine + 1, f))
	{
		tokenHost = strtok_r(lineHost, delim, &nextTokenHost);
		while (tokenHost != NULL)
		{
			if(strcmp(tokenHost, tokenGuest) == 0)
			{
				contOcurrencias++;
			}
			tokenHost = strtok_r(NULL, delim, &nextTokenHost);
		}
	}
	printf("Palabra: %s\tOcurrencias: %d\n", tokenGuest, contOcurrencias);

	free(lineHost);
	fclose(f);
}

/**
*
* Función para determinar si un token es un número correcto, ya sea positivo, negativo, decimal o entero
*
* Usada en: numeros
*
**/
short isNum(char* number){
    int i = 0;
    int contSigno = 0;
    int contPunto = 0;

	//Si la posicion 0 del token no es un número o un signo, no es un número
    if(number[0] != '-' && number[0] != '+' && number[0] != '.' && isdigit(number[0]) == 0)
	{
        return 0;
    }

	//Recorremos todo el token hasta llegar al final
    while(number[i] != '\0')
	{
		//Si es un número, debemos entrar aquí
        if(number[i] == '-' || number[i] == '+' || number[i] == '.' || isdigit(number[i]) > 0)
		{
			//Comprobamos la posición del signo
			if(number[i] == '-' || number[i] == '+')
			{
				//Si el signo no está al principio, no es un número válido
				if(i != 0)
				{
					return 0;
				}

				contSigno++;
			}

			if(number[i] == '.')
			{
				contPunto++;
			}
        }

		//Si no se encuentra un número o signo en ninguna posición del token, no es un número
		else
		{
            return 0;
        }
        i++;
    }

    if(contSigno > 1 || contPunto > 1)
	{
        //Si hay mas de un signo o mas de un punto devuelve falso porque no puede ser un numero
        return 0;
    }

    return 1;
}

/**
*
* Función para determinar si un token numérico es decimal o no
*
* Usada en: numeros
*
**/
short isDecimal(char* number)
{
	char* ret;

	//strchr encuentra la primera ocurrencia dentro de un string
	//Si no se encuentra el caracter, devuelve NULL
	ret = strchr(number, '.');
	if(ret != NULL)
	{
		return 1;
	}

	return 0;
}

/**
*
* Función para determinar si un token numérico es negativo o no
*
* Usada en: numeros
*
**/
short isNegative(char* number)
{
	char* ret;

	//strchr encuentra la primera ocurrencia dentro de un string
	//Si no se encuentra el caracter, devuelve NULL
	ret = strchr(number, '-');
	if(ret != NULL)
	{
		return 1;
	}

	return 0;
}

/**
*
* Función para determinar si un token es una fecha válida según el formato DD/MM/YYYY
*
* Usada en: fechas
*
**/
short isDate(char date[])
{
	if(strlen(date) < 10 || date[2] != '/' || date[5] != '/')
	{
		return 0;
	}
	
	for(int i=0 ; i < 10; i++)
	{
		//Saltamos las barras de separación
        if(i == 2 || i == 5)
		{
            i++;
        }

		//Si la posicion indicada no es un digito no es una fecha en formato dd/mm/aaaa
        if(isdigit(date[i]) == 0)
		{
            return 0;
        }
    }

	short arrayDates[2];
	short monthDays[] = {31, 28, 31, 30,31, 30, 31, 31, 30, 31, 30, 31};
	char* token = NULL;
	char* nextTokenDate = NULL;

	//Extraemos los días
	token = strtok_r(date, "/", &nextTokenDate);
	arrayDates[0] = atoi(token);

	//Extraemos el mes
	token = strtok_r(NULL, "/", &nextTokenDate);
	arrayDates[1] = atoi(token);

	//Extraemos el año
	token = strtok_r(NULL, "/", &nextTokenDate);
	arrayDates[2] = atoi(token);

	//Comprobamos si es un año bisiesto, cambiar Febrero en caso afirmativo
	if((arrayDates[2] % 4 == 0 && arrayDates[2] % 100 != 0) || arrayDates[2] % 400 == 0)
	{
		monthDays[1]++;
	}

	//Comprobamos mes
	if(arrayDates[1] < 1 || arrayDates[1] > 12)
	{
		return 0;
	}

	//Comprobamos día
	arrayDates[1]--;
	if(arrayDates[0] <= 0 || arrayDates[0] > monthDays[arrayDates[1]])
	{
		return 0;
	}

	//Devolvemos el mes que hemos quitado
	arrayDates[1]++;

	return 1;
}

/**
*
* Función para dar nombre a un fichero. Si ya existe el nombre, se le da otro hasta que se pueda crear
*
* Usada en: remplazar
*
**/
char* getNewFileName(char fileName[])
{
	FILE* fp;
    const char ch = '.';
    
    char *extension = NULL;
    char *aux = NULL;
    char *direccionArchivoOG = NULL;

	int n_aux = 0;
    int size = 0;

    aux = malloc(sizeof(char) * 100);
    if(aux == NULL)
	{
        perror("\nError al reservar memoria");
        printf("FIN\n");
        exit(EXIT_FAILURE);
    }
            
    direccionArchivoOG = malloc(sizeof(char) * 100);
    if(direccionArchivoOG == NULL)
	{
        perror("\nError al reservar memoria");
        printf("FIN\n");
        exit(EXIT_FAILURE);
    }

	strcpy(direccionArchivoOG, fileName);

	extension = strrchr(fileName, ch);

	//Comprobamos si el archivo tiene extensión
	if(extension == NULL)
	{
        strcat(fileName, "_new");
        fp = fopen(fileName, "r");

        if(fp)
		{
			int i = 2;

            //Se va iterando hasta que se encuentra un nombre que no esta en uso
            do
			{
				strcpy(fileName, direccionArchivoOG);
				int j = snprintf(fileName, strlen(fileName) + 15, "%s_new(%i)", fileName, i); //Con +15 caracteres obtenemos ficheros hasta 9999999 sin que se corte el nombre
                fp = fopen(fileName, "r");
				i++;
            }
			while(fp);
        }

    }
	else
	{

        //Si el archivo tiene extensión...
        n_aux = strlen(extension);

		strcpy(aux ,"");
        strcat(aux,"_new");
        strcat(aux, extension); //Almacenamos _new.extensión
    
        //Quitamos la extensión del nombre del fichero
        fileName[strlen(fileName) - n_aux] = '\0';  //Esta linea altera el valor de la cadena

        extension = strrchr(direccionArchivoOG, ch);     //extension por lo que se vuelve a calcular

        strcat(fileName, aux);
        fp = fopen(fileName, "r");

        if(fp)
		{
			//Si el fichero_new.extension ya existe...
            int i = 2;

            //Se va iterando hasta que se encuentra un nombre que no esta en uso
            do
			{
				strcpy(fileName,"");
				strcpy(fileName, direccionArchivoOG);
				fileName[strlen(fileName) - n_aux] = '\0';
				int j = snprintf(fileName, strlen(fileName) + 15, "%s_new(%i)%s", fileName, i, extension); //Con +15 caracteres obtenemos ficheros hasta 9999 sin que se corte el nombre
                fp = fopen(fileName, "r");
				i++;

            }
			while(fp);
        }
    }

	fclose(fp);
	free(aux);
	free(direccionArchivoOG);
	return fileName;
}

/**
*
* Función para dar reemplazar una palabra por otra. Se le debe pasar como parámetro:
* Un flujo de texto, palabra a encontrar, palabra que reemplazará a la encontrada, tamaño del buffer temporal
*
* Usada en: remplazar
*
**/
int replaceAll(char *str, const char *oldWord, const char *newWord, int maxLongLine)
{
    char *pos, *temp;
    int index = 0;
	int contReplace = 0;
    int owlen;

	temp = malloc((maxLongLine + 1) * sizeof(char));
	if (temp == NULL)
	{
		perror("\nError al reservar memoria");
		printf("\nFIN");
		exit(EXIT_FAILURE);
	}

    owlen = strlen(oldWord);
    //Repetimos hasta que todas las ocurrencias hayan sido reemplazadas
    while ((pos = strstr(str, oldWord)) != NULL)
    {
        // Guardamos la linea actual
        strcpy(temp, str);

        // Posición de la palabra encontrada
        index = pos - str;

        // Cortamos la linea hasta la palabra que hemos encontrado
        str[index] = '\0';

        // Concatenamos la linea con la palabra nueva
        strcat(str, newWord);
        
		// Concatenamos la linea nueva con el resto de palabras
        strcat(str, temp + index + owlen);
		
		//Aumentamos el contador de reemplazos
		contReplace++;
    }

	return contReplace;
}