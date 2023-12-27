#include "abb.h"
#include "abb_estructura_privada.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define COMPARACION_IGUAL 0

//Estructura auxiliar para la funcion de pasar un abb a array.
struct vector_con_tamanio {
	void **vector;
	size_t cantidad;
	size_t tope;
};

//PRE: - .
//POST: Inserta el nuevo nodo en la posicion correspondiente del ABB.
nodo_abb_t *abb_insertar_rec(abb_t *arbol, nodo_abb_t *nodo_actual,
			     void *elemento);

//PRE: El nodo recibido no puede ser NULL.
//POST: Elimina el nodo predecesor y devuelve el elemento del mismo.
nodo_abb_t *eliminar_predecesor(nodo_abb_t *raiz, void **elemento_predecesor);

//PRE: - .
//POST: Elimina el nodo, devuelve el elemento y hace los reemplazos necesarios para resguardar la integridad del ABB.
nodo_abb_t *abb_quitar_rec(nodo_abb_t *raiz, void *elemento,
			   void **elemento_borrar, abb_t *arbol);

//PRE: - .
//POST: Devuelve el elemento igual al recibido por parametro si lo encuentra, o NULL en caso contrario.
void *abb_buscar_rec(nodo_abb_t *raiz, void *elemento, abb_t *arbol);

//PRE: - .
//POST: Libera la memoria en el HEAP de todos los nodos del abb.
void abb_destruir_rec(nodo_abb_t *raiz);

//PRE: La funcion destructora no debe ser NULL.
//POST: Libera la memoria en el HEAP de todos los nodos del abb y la de sus elementos.
void abb_destruir_todo_rec(nodo_abb_t *raiz, void (*destructor)(void *));

//PRE: La funcion recibida no debe ser nula.
//POST: Aplica la funcion recibida por parametro a todos los elementos posibles recorriendo de forma "INORDEN".
bool abb_iterar_inorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			bool (*funcion)(void *, void *), void *aux,
			size_t *contador);

//PRE: La funcion recibida no debe ser nula.
//POST: Aplica la funcion recibida por parametro a todos los elementos posibles recorriendo de forma "PREORDEN".
bool abb_iterar_preorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			 bool (*funcion)(void *, void *), void *aux,
			 size_t *contador);

//PRE: La funcion recibida no debe ser nula.
//POST: Aplica la funcion recibida por parametro a todos los elementos posibles recorriendo de forma "POSTORDEN".
bool abb_iterar_postorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			  bool (*funcion)(void *, void *), void *aux,
			  size_t *contador);

//PRE: El aux recibido por parametro no puede ser NULL.
//POST: Aniade el elemento al array, siempre y cuando la cantidad de elementos sea menor al tope.
bool agrandar_vector(void *elemento, void *aux);

nodo_abb_t *abb_insertar_rec(abb_t *arbol, nodo_abb_t *nodo_actual,
			     void *elemento)
{
	if (nodo_actual == NULL) {
		nodo_abb_t *nodo_actual = calloc(1, sizeof(nodo_abb_t));
		nodo_actual->elemento = elemento;
		return nodo_actual;
	}

	int comparacion = arbol->comparador(elemento, nodo_actual->elemento);

	if (comparacion <= COMPARACION_IGUAL) {
		nodo_actual->izquierda = abb_insertar_rec(
			arbol, nodo_actual->izquierda, elemento);
	} else {
		nodo_actual->derecha =
			abb_insertar_rec(arbol, nodo_actual->derecha, elemento);
	}
	return nodo_actual;
}

nodo_abb_t *eliminar_predecesor(nodo_abb_t *raiz, void **elemento_predecesor)
{
	if (raiz->derecha == NULL) {
		*elemento_predecesor = raiz->elemento;
		nodo_abb_t *izquierda = raiz->izquierda;
		free(raiz);
		return izquierda;
	}

	raiz->derecha = eliminar_predecesor(raiz->derecha, elemento_predecesor);
	return raiz;
}

nodo_abb_t *abb_quitar_rec(nodo_abb_t *raiz, void *elemento,
			   void **elemento_borrar, abb_t *arbol)
{
	if (raiz == NULL) {
		return NULL;
	}

	int comparacion = arbol->comparador(elemento, raiz->elemento);

	if (comparacion < COMPARACION_IGUAL) {
		raiz->izquierda = abb_quitar_rec(raiz->izquierda, elemento,
						 elemento_borrar, arbol);
	} else if (comparacion > COMPARACION_IGUAL) {
		raiz->derecha = abb_quitar_rec(raiz->derecha, elemento,
					       elemento_borrar, arbol);
	} else {
		*elemento_borrar = raiz->elemento;
		if (raiz->derecha == NULL || raiz->izquierda == NULL) {
			nodo_abb_t *hijo_no_nulo = raiz->derecha;
			if (hijo_no_nulo == NULL) {
				hijo_no_nulo = raiz->izquierda;
			}

			free(raiz);
			arbol->tamanio -= 1;
			return hijo_no_nulo;
		} else {
			void *elemento_predecesor = NULL;
			raiz->izquierda = eliminar_predecesor(
				raiz->izquierda, &elemento_predecesor);
			raiz->elemento = elemento_predecesor;
			arbol->tamanio -= 1;
			return raiz;
		}
	}
	return raiz;
}

void *abb_buscar_rec(nodo_abb_t *raiz, void *elemento, abb_t *arbol)
{
	if (raiz == NULL) {
		return NULL;
	}

	int comparacion = arbol->comparador(elemento, raiz->elemento);

	if (comparacion > COMPARACION_IGUAL) {
		return abb_buscar_rec(raiz->derecha, elemento, arbol);
	} else if (comparacion < COMPARACION_IGUAL) {
		return abb_buscar_rec(raiz->izquierda, elemento, arbol);
	} else {
		return raiz->elemento;
	}
}

void abb_destruir_rec(nodo_abb_t *raiz)
{
	if (raiz == NULL) {
		return;
	}

	abb_destruir_rec(raiz->izquierda);
	abb_destruir_rec(raiz->derecha);
	free(raiz);
}

void abb_destruir_todo_rec(nodo_abb_t *raiz, void (*destructor)(void *))
{
	if (raiz == NULL) {
		return;
	}

	abb_destruir_todo_rec(raiz->izquierda, destructor);
	abb_destruir_todo_rec(raiz->derecha, destructor);
	destructor(raiz->elemento);
	free(raiz);
}

bool abb_iterar_inorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			bool (*funcion)(void *, void *), void *aux,
			size_t *contador)
{
	if (!raiz) {
		return true;
	}

	bool continuar = abb_iterar_inorden(raiz->izquierda, recorrido, funcion,
					    aux, contador);
	if (continuar == false) {
		return false;
	}

	(*contador)++;
	if (funcion(raiz->elemento, aux) == false) {
		return false;
	}

	return abb_iterar_inorden(raiz->derecha, recorrido, funcion, aux,
				  contador);
}

bool abb_iterar_preorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			 bool (*funcion)(void *, void *), void *aux,
			 size_t *contador)
{
	if (!raiz) {
		return true;
	}

	(*contador)++;
	if (funcion(raiz->elemento, aux) == false) {
		return false;
	}

	bool continuar = abb_iterar_preorden(raiz->izquierda, recorrido,
					     funcion, aux, contador);
	if (continuar == false) {
		return false;
	}

	continuar = abb_iterar_preorden(raiz->derecha, recorrido, funcion, aux,
					contador);
	if (continuar == false) {
		return false;
	}

	return true;
}

bool abb_iterar_postorden(nodo_abb_t *raiz, abb_recorrido recorrido,
			  bool (*funcion)(void *, void *), void *aux,
			  size_t *contador)
{
	if (!raiz) {
		return true;
	}

	bool continuar = abb_iterar_postorden(raiz->izquierda, recorrido,
					      funcion, aux, contador);
	if (continuar == false) {
		return false;
	}

	continuar = abb_iterar_postorden(raiz->derecha, recorrido, funcion, aux,
					 contador);
	if (continuar == false) {
		return false;
	}

	(*contador)++;
	if (funcion(raiz->elemento, aux) == false) {
		return false;
	}

	return true;
}

bool agrandar_vector(void *elemento, void *aux)
{
	struct vector_con_tamanio *estructura_auxiliar =
		(struct vector_con_tamanio *)aux;

	if (estructura_auxiliar->cantidad >= estructura_auxiliar->tope) {
		return false;
	}

	estructura_auxiliar->vector[estructura_auxiliar->cantidad] = elemento;
	estructura_auxiliar->cantidad++;
	return true;
}

abb_t *abb_crear(abb_comparador comparador)
{
	if (!comparador) {
		return NULL;
	}

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb) {
		return NULL;
	}

	abb->comparador = comparador;
	abb->tamanio = 0;

	return abb;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	arbol->nodo_raiz = abb_insertar_rec(arbol, arbol->nodo_raiz, elemento);
	arbol->tamanio += 1;
	return arbol;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}

	void *elemento_borrar = NULL;
	arbol->nodo_raiz = abb_quitar_rec(arbol->nodo_raiz, elemento,
					  &elemento_borrar, arbol);

	return elemento_borrar;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (!arbol) {
		return NULL;
	}
	if (arbol->nodo_raiz == NULL) {
		return NULL;
	}

	void *elemento_encontrado =
		abb_buscar_rec(arbol->nodo_raiz, elemento, arbol);
	return elemento_encontrado;
}

bool abb_vacio(abb_t *arbol)
{
	if (!arbol) {
		return true;
	}
	if (arbol->tamanio == 0) {
		return true;
	}

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (!arbol) {
		return 0;
	}

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (!arbol) {
		return;
	}

	abb_destruir_rec(arbol->nodo_raiz);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (!arbol) {
		return;
	}
	if (!destructor) {
		free(arbol);
		return;
	}

	abb_destruir_todo_rec(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (!arbol) {
		return 0;
	}
	if (!funcion) {
		return 0;
	}

	size_t contador = 0;

	if (recorrido == INORDEN) {
		abb_iterar_inorden(arbol->nodo_raiz, recorrido, funcion, aux,
				   &contador);
	} else if (recorrido == PREORDEN) {
		abb_iterar_preorden(arbol->nodo_raiz, recorrido, funcion, aux,
				    &contador);
	} else if (recorrido == POSTORDEN) {
		abb_iterar_postorden(arbol->nodo_raiz, recorrido, funcion, aux,
				     &contador);
	}

	return contador;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (!arbol) {
		return 0;
	}
	if (!array) {
		return 0;
	}
	if (tamanio_array == 0) {
		return 0;
	}

	struct vector_con_tamanio aux;
	aux.vector = array;
	aux.tope = tamanio_array;
	aux.cantidad = 0;

	if (recorrido == INORDEN) {
		abb_con_cada_elemento(arbol, recorrido, agrandar_vector, &aux);
	} else if (recorrido == PREORDEN) {
		abb_con_cada_elemento(arbol, recorrido, agrandar_vector, &aux);
	} else if (recorrido == POSTORDEN) {
		abb_con_cada_elemento(arbol, recorrido, agrandar_vector, &aux);
	}

	return aux.cantidad;
}