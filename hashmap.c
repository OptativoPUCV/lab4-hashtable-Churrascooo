#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}

//---------------------------------------------------------
void insertMap(HashMap * map, char * key, void * value)
{
  long pos;
  if (map == NULL || key == NULL)
  {
    return;
  }
  pos = hash(key, map->capacity);
  
  while (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL)
  {
    if (is_equal(map->buckets[pos]->key, key))
    {
      map->buckets[pos]->value = value;
    }
    pos = (pos + 1) % map->capacity; //avanza al siguiente índice en el mapa.
  }
  map->buckets[pos] = createPair(key, value);
  map->size++;
  map->current = pos;
}
//---------------------------------------------------------
void enlarge(HashMap * map)
{
  enlarge_called = 1; //no borrar (testing purposes)
  
  if (map == NULL)
  {
    return;
  }
  Pair ** oldBuckets = map->buckets;
  long oldCapacity = map->capacity;
  map->capacity *= 2;
  map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));

  map->size = 0;
  for (int i = 0 ; i < oldCapacity ; i++)
  {
    if (oldBuckets[i] != NULL)
    {
      insertMap(map, oldBuckets[i]->key, oldBuckets[i]->value);
      (map->size)++;  
    }
  }
  free(oldBuckets);
}
//---------------------------------------------------------
HashMap * createMap(long capacity)
{ 
  HashMap *mapa = (HashMap *)malloc(sizeof(HashMap));
  
  mapa->buckets = (Pair **)calloc(capacity, sizeof(Pair *));

  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;

  return mapa;
    return NULL;
}
//---------------------------------------------------------
void eraseMap(HashMap * map,  char * key)
{
  long pos;
  pos = hash(key, map->capacity);
  if (strcmp(map->buckets[pos]->key, key) == 0)
  {
    map->buckets[pos]->key = NULL;
    map->size--;
  }
  else
  {
    while (map->buckets[pos] != NULL)
      {
        if (strcmp(map->buckets[pos]->key, key) == 0)
        {
          map->buckets[pos]->key = NULL;
          map->size--;
          return;
        }
        pos = (pos+1) % map->capacity;
      }
  }
}
//---------------------------------------------------------
Pair * searchMap(HashMap * map,  char * key)
{
  long pos = hash(key, map->capacity); //Se usa hash para encontrar la posición.
  while (map->buckets[pos] != NULL)
  {
    if (strcmp(map->buckets[pos]->key, key) == 0)
    {
      map->current = pos;
      return map->buckets[pos];
    }
    pos = (pos + 1) % map->capacity; //avanza al siguiente índice en el mapa.
  }
  return NULL;  
}
//---------------------------------------------------------
Pair * firstMap(HashMap * map)
{
  if (map == NULL)
  {
    return NULL;
  }
  for (long i = 0 ; i < map->capacity; i++)
  {
    if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
//---------------------------------------------------------
Pair * nextMap(HashMap * map)
{
  if (map == NULL)
  {
    return NULL;
  }

  long posicion = map->current + 1;

  for (long i = posicion ; i < map->capacity ; i++)
  {
    if (map->buckets[i] != NULL)
    {
      map->current = i;
      return map->buckets[i];
    }
  }
  return NULL;
}
//---------------------------------------------------------