#include "pruebas.h"
#include "src/abb.h"
#include "src/abb_estructura_privada.h"
#include <stdio.h>

#define MENOR -1
#define IGUAL 0
#define MAYOR 1
#define APLICACIONES_ESPERADAS 0
#define TAMANIO_CARGA_ESPERADO 3
#define TAMANIO_CARGA_ESPERADO_2 5
#define TAMANIO_BORRAR_ESPERADO 3
#define TAMANIO_ESPERADO_POR_FUNCION 3
#define CANTIDAD_RECORRIDA_ESPERADA 3
#define VACIO 0

// PRE: Los elementos recibidos deben ser enteros.
// POST: Devuelve "-1" si el valor 1 es menor al valor 2, "0" si los
//       elementos valen lo mismo o "1" si el valor 1 es mayor al valor 2.
int comparador(void *valor1, void *valor2);

int comparador(void *valor1, void *valor2) {
  if (!valor1) {
    return -2;
  }
  if (!valor2) {
    return -2;
  }

  int int_valor1 = *(int *)valor1;
  int int_valor2 = *(int *)valor2;

  if (int_valor1 < int_valor2) {
    return MENOR;
  } else if (int_valor1 == int_valor2) {
    return IGUAL;
  } else {
    return MAYOR;
  }
}

void crear_con_comparador_nulo() {
  abb_t *abb = abb_crear(NULL);
  pruebas_afirmar(abb == NULL, "No se puede crear ABB con comparador nulo.\n");
}

void agregar_nodo_abb_nulo() {
  int valor_prueba = 1;
  void *puntero_prueba = &valor_prueba;
  abb_t *abb = abb_insertar(NULL, puntero_prueba);
  pruebas_afirmar(abb == NULL, "No se puede insertar en un ABB nulo.\n");
}

void no_puedo_eliminar_en_abb_nulo() {
  int valor1 = 1;
  void *puntero1 = &valor1;
  void *elemento_prueba = abb_quitar(NULL, puntero1);
  pruebas_afirmar(elemento_prueba == NULL,
                  "No se puede borrar en un ABB nulo.\n");
}

void no_elimina_si_no_esta_el_elemento() {
  abb_t *arbol = abb_crear(comparador);
  int valor_prueba_1 = 20;
  int valor_buscado = 12;
  void *puntero_1 = &valor_prueba_1;
  void *elemento_buscado = &valor_buscado;
  arbol = abb_insertar(arbol, puntero_1);
  if (arbol->nodo_raiz != NULL) {
    abb_quitar(arbol, elemento_buscado);
  }
  pruebas_afirmar(arbol->nodo_raiz != NULL,
                  "No se puede eliminar un elemento que no esta en el ABB.\n");
  abb_destruir(arbol);
}

void no_se_busca_en_abb_nulo() {
  int valor1 = 1;
  void *puntero_1 = &valor1;
  void *elemento_prueba = abb_buscar(NULL, puntero_1);
  pruebas_afirmar(elemento_prueba == NULL,
                  "No se puede buscar un elemento en un ABB nulo.\n");
}

void no_se_encuentra_elemento() {
  abb_t *arbol = abb_crear(comparador);
  int valor_prueba_1 = 20;
  int valor_buscado = 12;
  void *puntero_1 = &valor_prueba_1;
  void *elemento_buscado = &valor_buscado;
  arbol = abb_insertar(arbol, puntero_1);
  void *elemento_conseguido = abb_buscar(arbol, elemento_buscado);
  pruebas_afirmar(elemento_conseguido == NULL,
                  "El elemento no se encuentra en el ABB.\n");
  abb_destruir(arbol);
}

void abb_nulo_esta_vacio() {
  bool vacio = abb_vacio(NULL);
  pruebas_afirmar(vacio == true, "Un ABB nulo siempre esta vacio.\n");
}

void abb_nulo_no_tiene_nodos() {
  size_t cantidad = abb_tamanio(NULL);
  pruebas_afirmar(cantidad == VACIO, "Un ABB nulo no tiene elementos.\n");
}

void abb_cada_elemento_y_recorrido_nulos() {
  abb_t *arbol_aux = abb_crear(comparador);
  int valor_prueba = 5;
  void *puntero = &valor_prueba;
  abb_recorrido recorrido = INORDEN;
  arbol_aux = abb_insertar(arbol_aux, puntero);
  size_t aplicaciones =
      abb_con_cada_elemento(arbol_aux, recorrido, NULL, puntero);
  pruebas_afirmar(aplicaciones == APLICACIONES_ESPERADAS,
                  "No se aplica funcion NULA a elementos del ABB.\n");
  size_t tamanio_aux = 10;
  aplicaciones = abb_recorrer(arbol_aux, recorrido, NULL, tamanio_aux);
  pruebas_afirmar(aplicaciones == APLICACIONES_ESPERADAS,
                  "No se recorre un ABB si el array es nulo.\n");
  abb_destruir(arbol_aux);
}

abb_t *crear_abb(abb_t *arbol) {
  arbol = abb_crear(comparador);
  pruebas_afirmar(arbol != NULL, "El ABB se inicializo correctamente.\n");
  return arbol;
}

abb_t *test_inserciones_abb(abb_t *arbol) {
  int valor_prueba_1 = 5;
  int valor_prueba_2 = 2;
  int valor_prueba_3 = 10;
  void *puntero_1 = &valor_prueba_1;
  void *puntero_2 = &valor_prueba_2;
  void *puntero_3 = &valor_prueba_3;
  arbol = abb_insertar(arbol, puntero_1);
  if (arbol->nodo_raiz != NULL) {
    arbol = abb_insertar(arbol, puntero_2);
    if (arbol->nodo_raiz->izquierda != NULL) {
      arbol = abb_insertar(arbol, puntero_3);
    }
  }
  pruebas_afirmar(arbol->nodo_raiz->derecha != NULL,
                  "Se pudo insertar 3 elementos en los lugares correctos.\n");
  return arbol;
}

void verificar_elementos_cargados(abb_t *arbol) {
  bool elementos_cargados = false;
  if (arbol->nodo_raiz->elemento != NULL) {
    if (arbol->nodo_raiz->izquierda->elemento != NULL) {
      if (arbol->nodo_raiz->derecha->elemento != NULL) {
        elementos_cargados = true;
      }
    }
  }
  pruebas_afirmar(elementos_cargados == true,
                  "Los nodos fueron cargados con sus elementos.\n");
}

void verificar_tamanio_estructura(abb_t *arbol) {
  pruebas_afirmar(arbol->tamanio == TAMANIO_CARGA_ESPERADO,
                  "El tamanio del ABB es el correcto.\n");
}

void eliminar_elementos_abb(abb_t *arbol) {
  int valor_prueba_1 = 3;
  int valor_prueba_2 = 11;
  void *puntero_1 = &valor_prueba_1;
  void *puntero_2 = &valor_prueba_2;
  arbol = abb_insertar(arbol, puntero_1);
  arbol = abb_insertar(arbol, puntero_2);
  void *resultado_1 = abb_quitar(arbol, puntero_1);
  void *resultado_2 = abb_quitar(arbol, puntero_2);
  pruebas_afirmar(resultado_1 != resultado_2,
                  "Se pueden eliminar nodos en el ABB sin problemas.\n");
}

void encontrar_elemento_cargado(abb_t *arbol) {
  int valor_prueba_1 = 28;
  int inicializador_punteros = 0;
  void *puntero_1 = &valor_prueba_1;
  arbol = abb_insertar(arbol, puntero_1);
  void *elemento_conseguido = &inicializador_punteros;
  void *puntero_2 = &inicializador_punteros;
  elemento_conseguido = abb_buscar(arbol, puntero_1);
  pruebas_afirmar(elemento_conseguido != puntero_2,
                  "Se ha encontrado el elemento dentro del ABB.\n");
}

void abb_no_esta_vacio(abb_t *arbol) {
  bool vacio = true;
  vacio = abb_vacio(arbol);
  pruebas_afirmar(vacio == false,
                  "El ABB cargado previamente no esta vacio.\n");
}

void abb_tamanio_funcion(abb_t *arbol) {
  size_t tamanio = abb_tamanio(arbol);
  printf("TAMANIO ACTUAL %zu\n", tamanio);
  pruebas_afirmar(
      tamanio == TAMANIO_ESPERADO_POR_FUNCION,
      "La funcion de tamanio esta devolviendo el valor correcto.\n");
}

// Esta funcion sirve para no enviar un NULL a abb_con_cada_elemento y que puede
// funcionar el test de abajo.
bool funcion_aux(void *elemento1, void *elemento2) { return true; }

void test_recorrido_abb() {
  int valor1 = 1;
  int valor2 = 2;
  int valor3 = 3;
  void *puntero1 = &valor1;
  void *puntero2 = &valor2;
  void *puntero3 = &valor3;

  abb_t *arbol = abb_crear(comparador);

  abb_insertar(arbol, puntero1);
  abb_insertar(arbol, puntero2);
  abb_insertar(arbol, puntero3);

  size_t elementos_iterados =
      abb_con_cada_elemento(arbol, INORDEN, funcion_aux, NULL);
  pruebas_afirmar(
      elementos_iterados == CANTIDAD_RECORRIDA_ESPERADA,
      "Se aplico la funcion a la cantidad de elementos correcta del ABB.\n");

  int *vector[5];
  size_t tamanio_maximo = 10;
  size_t elementos_iterados_2 =
      abb_recorrer(arbol, PREORDEN, (void **)vector, tamanio_maximo);
  pruebas_afirmar(elementos_iterados_2 == CANTIDAD_RECORRIDA_ESPERADA,
                  "Se anadio la cantidad esperada de elementos al array.\n");

  abb_destruir(arbol);
}

void pruebas_null_o_error() {
  crear_con_comparador_nulo();
  agregar_nodo_abb_nulo();
  no_puedo_eliminar_en_abb_nulo();
  no_elimina_si_no_esta_el_elemento();
  no_se_busca_en_abb_nulo();
  no_se_encuentra_elemento();
  abb_nulo_esta_vacio();
  abb_nulo_no_tiene_nodos();
  abb_cada_elemento_y_recorrido_nulos();
}

void pruebas_funcionamiento() {
  abb_t *arbol = NULL;
  arbol = crear_abb(arbol);
  arbol = test_inserciones_abb(arbol);
  verificar_elementos_cargados(arbol);
  verificar_tamanio_estructura(arbol);
  eliminar_elementos_abb(arbol);
  encontrar_elemento_cargado(arbol);
  abb_no_esta_vacio(arbol);
  abb_tamanio_funcion(arbol);
  test_recorrido_abb();
  abb_destruir(arbol);
}

int main() {
  pruebas_nuevo_grupo(
      "\n======================== XXX ========================");
  pruebas_nuevo_grupo("\nPruebas de casos NULL o ERROR.");
  pruebas_null_o_error();
  pruebas_nuevo_grupo("\nPruebas de funcionamiento del ABB.");
  pruebas_funcionamiento();
  pruebas_nuevo_grupo(
      "\n======================== XXX ========================");

  return pruebas_mostrar_reporte();
}
