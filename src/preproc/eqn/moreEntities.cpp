#include "eqnx.h"

void AddMoreEntities() {

  spacing_type cellType = s_ordinary;

  g_entityMap.add("u2234", "&#x2234;", cellType); // therefore
  g_entityMap.add("u2235", "&#x2235;", cellType); // because
  g_entityMap.add("u2236", "&#x2236;", cellType); // ratio
  cellType = s_operator;
  g_entityMap.add("u2237", "&#x2237;", cellType); // proportional
  g_entityMap.add("u2284", "&#x2284;", cellType); // nsubset
  g_entityMap.add("u2285", "&#x2285;", cellType); // nsupset
  g_entityMap.add("u2288", "&#x2288;", cellType); // nsubseteq
  cellType = s_ordinary;
  g_entityMap.add("u2289", "&#x2289;", cellType); // nsupseteq
  g_entityMap.add("u2329", "&#x2329;", cellType); // langle
  g_entityMap.add("u232A", "&#x232A;", cellType); // rangle
  g_entityMap.add("u27E8", "&#x27E8;", cellType); // bra
  g_entityMap.add("u27E9", "&#x27E9;", cellType); // ket
  cellType = s_operator;
  g_entityMap.add("u2204", "&#x2204;", cellType); // nexists
  g_entityMap.add("u2209", "&#x2209;", cellType); // nin
  g_entityMap.add("u220C", "&#x220C;", cellType); // nni
  g_entityMap.add("u2218", "&#x2218;", cellType); // ring
  cellType = s_ordinary;
  g_entityMap.add("u00B0", "&#xB0;", cellType); // deg
  g_entityMap.add("u221F", "&#x221F;", cellType); // anglert
  g_entityMap.add("u00FE", "&#xFE;", cellType); // thorn
  g_entityMap.add("u00DE", "&#xDE;", cellType); // Thorn
  g_entityMap.add("u00F0", "&#xF0;", cellType); // eth
  g_entityMap.add("u00D0", "&#xD0;", cellType); // Eth
}