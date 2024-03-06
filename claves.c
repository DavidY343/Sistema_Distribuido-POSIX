
#include "claves.h"
#include "list.h"

int init()
{
	destroy(&messages);
	init_list(&messages);
}

int set_value(int key, char *value1, int N_value2, double *V_value2)
{
	if (exist_element(messages, key) == 0)
		return (-1);
	if (N_value2 > 32 || N_value2 < 1)
		return (-1);
	if (set(&messages, value1, key, N_value2, V_value2) == 0)
		return (0);
	else
		return (-1);
}

/*Tanto value1 como V_value2 tienen que tener
 * espacio reservado para poder almacenar el máximo número de elementos posible (256 en el caso
 * de la cadena de caracteres y 32 en el caso del vector de doubles)*/
//Es lo que falta por implementar
int get_value(int key, char *value1, int *N_value2, double *V_value2)
{
	if (get_element(messages, key, value1, N_value2, V_value2) == 0)
		return (0);
	else
		return (-1);
}

int modify_value(int key, char *value1, int N_value2, double *V_value2)
{
	if (exist_element(messages, key) == -1)
		return (-1);
	if (N_value2 > 32 || N_value2 < 1)
		return (-1);
	if (modify_element(messages, key, value1, N_value2, V_value2) == 0)
		return (0);
	else
		return (-1);
}

int delete_key(int key)
{
	if (exist_element(messages, key) == -1)
		return (-1);
	if (delete_element(&messages, key) == 0)
		return (0);
	else
		return (-1);	
}

int exist(int key)
{
	if (exist_element(messages, key) == 0)
		return (0);
	else
		return (-1);
}
