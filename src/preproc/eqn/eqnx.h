#ifndef EQNX_H
#define EQNX_H

#include <map>
#include <string>

/*
Move some declarations to a header file in order to keep most of the
definitions related to eqnx separate from legacy eqn code.
*/

/*
Given the name of the macro and its contents, add to the hash of macros.
*/
extern void addEqnMacro(const char *name, const char *action);


// Moved to header so map can have a spacing_type. The reason for the move is to
// know the difference between operators and identifiers for doing MathML
enum spacing_type {
  s_ordinary,
  s_operator,
  s_binary,
  s_relation,
  s_opening,
  s_closing,
  s_punctuation,
  s_inner,
  s_suppress
};

// I think I can still safely use char pointers for from and to here, since
// if these are copied, it works if the ultimate source is a statically
// declared character string
struct EntityCell {
	const char *from;
	const char *to;
	spacing_type stype;
	EntityCell() {
		from = NULL;
		to = NULL;
		stype = s_ordinary;
	};
	EntityCell(const char *lfrom, const char *lto, spacing_type lstype = s_ordinary) {
		from = lfrom;
		to = lto;
		stype = lstype;
	};
	EntityCell(const EntityCell &that) {
		from = that.from;
		to = that.to;
		stype = that.stype;
	};
	EntityCell &operator=(const EntityCell &that) {
		from = that.from;
		to = that.to;
		stype = that.stype;
	};
};

class EntityMap {
	// use a string in order to do proper comparisons
	// the values are value types! Copying happens.
	std::map<std::string,EntityCell> m_cells;

public:
	void add(EntityCell &cell) {
		m_cells[cell.from] = cell;
	}
	void add(const char *lfrom, const char *lto, spacing_type lstype = s_ordinary) {
		EntityCell cell(lfrom, lto, lstype);
		m_cells[lfrom] = cell;
	}
	EntityCell &get(const char *key) {
		return m_cells[key];
	}


};

extern EntityMap &g_entityMap;

extern void AddTexMacros();
extern void AddTexEntities();
extern void AddMoreMacros();
extern void AddMoreEntities();
extern void AddDsMacros();
extern void AddDsEntities();
extern void AddFrakturMacros();
extern void AddFrakturEntities();
extern void AddScriptMacros();
extern void AddScriptEntities();
extern void AddBoldFrakturMacros();
extern void AddBoldFrakturEntities();
extern void AddBoldScriptMacros();
extern void AddBoldScriptEntities();
extern void AddSansSerifMacros();
extern void AddSansSerifEntities();
extern void AddBoldSansSerifMacros();
extern void AddBoldSansSerifEntities();


	
#endif