#include "client.h"
#include <readline/readline.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	valor = config_get_string_value(config, "CLAVE");
	//printf(valor);
	ip = config_get_string_value(config, "IP");
	//printf(ip);
	puerto = config_get_string_value(config, "PUERTO");
	//printf(puerto);

	// Loggeamos el valor de config

	log_info(logger, valor);
	log_info(logger, ip);
	log_info(logger, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	printf("leo por consola y solo loggeo hasta eviar un string vacio \n");
	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	/////////////////////////////HANDSHAKE/////////////////////////////

	size_t bytes;

	int32_t handshake = 1;
	int32_t result;

	bytes = send(conexion, &handshake, sizeof(int32_t), 0);
	bytes = recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);

	if (result == 0) {
		// Handshake OK
		printf("el handshake se realizó correctamente \n");
	} else {
		// Handshake ERROR
		printf("falló el handshake \n");
	}
	
	/////////////////////////////HANDSHAKE/////////////////////////////
	
	// Enviamos al servidor el valor de CLAVE como mensaje
	printf("envio al servidor el valor de CLAVE como mensaje \n");
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	printf("empiezo a armar el paquete que luego le envio al servidor con un string vacio \n");
	paquete(conexion);

	printf("llego a esta linea y se termina el programa \n");
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log", "logTP0", true, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL){
		printf("hubo un problema al buscar o encontrar el log");
		exit(EXIT_FAILURE);
	} //else{printf("log creado");}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/repos/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		printf("hubo un problema al crear el config");
		exit(EXIT_FAILURE);
	} //else{printf("config creado");}

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa

	leido = readline("> ");

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío

	while (strcmp(leido, "")){ //strcmp devuelve 0 (false, sale del while) si los strings son iguales y sino un numero q ahora no importa, pero ese numero se toma como true y entra al while
		log_info(logger, leido);
		free(leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!

	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while (strcmp(leido, "")){ //strcmp devuelve 0 (false, sale del while) si los strings son iguales y sino un numero q ahora no importa, pero ese numero se toma como true y entra al while
		agregar_a_paquete(paquete, leido, strlen(leido)+1);
		free (leido);
		leido = readline("> ");
	}
	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */

	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
