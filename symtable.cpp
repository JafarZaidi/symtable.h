/*symtable.h*/

// 
// Jafar Zaidi
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #03: symtable
//
// Symbol Table: a symbol table is a stack of scopes, typically used by a
// compiler to keep track of symbols in a program (functions, variables,
// types, etc.).  In most programming languages, you "enter scope" when you 
// see {, and "exit scope" when you see the corresponding }.  Example:
//
// int main()
// {              <-- enterScope()
//    int i;      <-- enter "i" into symbol table as type "int"
//    .
//    .
//    while (true)
//    {              <-- enterScope()
//       char i;     <-- enter "i" into symbol table as type "char"
//
// Notice there are two variables named "i", which is legal because
// they are in different scopes.
//

#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <utility>
using namespace std;

template<typename KeyT, typename SymbolT>
class symtable
{
public:
  //
  // A symbol table is a stack of scopes.  Every scope has a name, and 
  // we use a map to hold the symbols in that scope.  You can *add* to
  // this class, but you must use the Name and Symbols as given to store
  // the scope name, and scope symbols, respectively.
  //
  class Scope
  {
  public:
    string              Name;
    map<KeyT, SymbolT>  Symbols;

    // constructors:
    Scope()
    {
		Name = "Not Given";
    }

    Scope(string name)
    {
      this->Name = name;
      // empty map created by map's constructor:
    }
	//Symbols is a map of names and symbols. Therefore, the number of symbols (and names) must be equal to its size.
	//This is a getter method that will allow us to access the aforementioned number in symtable
	int numSymbols(){
		return Symbols.size();
	}
  };

private:
	deque<Scope> symboltable;
public:
  enum class ScopeOption
  {
    ALL, 
    CURRENT,
    GLOBAL
  };

  //
  // default constructor:
  //
  // Creates a new, empty symbol table.  No scope is open.
  //

  symtable()
  {
    
  }

  //
  // size
  //
  // Returns total # of symbols in the symbol table.
  //
  // Complexity: O(1)
  //
  
  // we access each scope in symboltable (which is a deque of scopes) and extract the number of symbols within 
  // each individual scope. We then add that number to a variable. By the time the for each loop ends, our variable
  // "count" will have the entire symboltable's number of symbols.
  int size() 
  {
	int count = 0;
    for(Scope& s : symboltable){
		count+= s.numSymbols();
	}
	return count;
  }

  //
  // numscopes
  //
  // Returns the # of open scopes.
  //
  // Complexity: O(1)
  //
  
  /* symboltable is a deque of scopes. If we want to get rid of a scope, we'd effectively have to remove it 
   * from the deque. Therefore, whatever is symboltable's current size is the number of open scopes. */
  int numscopes() const
  {
    return symboltable.size();
  }

  //
  // enterScope
  //
  // Enters a new, open scope in the symbol table, effectively "pushing" on
  // a new scope.  You must provide a name for the new scope, although
  // the name is currently used only for debugging purposes.
  //
  // NOTE: the first scope you enter is known as the GLOBAL scope, since this
  // is typically where GLOBAL symbols are stored.  
  //
  // Complexity: O(1)
  //
  
  /* Since symboltable is a deque of scopes, we can use the "push_front" function to add a new scope to the deque.
   * Since we're naming it, simply send the constructor name as an argument of Scope and the new Scope will be added.
   * Note that we could've added to the back instead, but that's lame so I didn't do that. */
   
  void enterScope(string name)
  {
	  symboltable.push_front(Scope(name));
  }

  //
  // exitScope
  //
  // Exits the current open scope, discarding all symbols in this scope.
  // This effectively "pops" the symbol table so that it returns to the 
  // previously open scope.  A runtime_error is thrown if no scope is 
  // currently open.  
  //
  // Complexity: O(1)
 
  /* Since we added scopes to the front of the deque, We must remove them from the front too using pop. Oh, and you
   * don't want the program to crash because there are no scopes in symboltable? Then you have to check that
   * symboltable's size is greater than zero. I decided to throw a runtime error, but that isn't necessary. */
   
  void exitScope()
  {
    if(symboltable.size()>0){
		symboltable.pop_front();
	}
	else{
		throw runtime_error("Symbol table is empty");
	}
  }

  //
  // curScope
  //
  // Returns a copy of the current scope.  A runtime_error is thrown if
  // no scope is currently open.
  //
  // Complexity: O(N) where N is the # of symbols in the current scope
  //
  /* This is pretty simple. Since we're adding scopes to the front. The current scope will be on front,
   * so just return the front Scope in symboltable. Also check that symboltable's size is greater than zero.
   * Can't return the current scope if there aren't any after all */
  Scope curScope() const
  {
	if(symboltable.size()>0){
		return symboltable.front();
	}
	else{
		throw runtime_error("Symbol table is empty");
	}
  }

  //
  // insert
  //
  // Inserts the (key, symbol) pair in the *current* scope.  If the key
  // already exists in the current scope, the associated symbol is replaced
  // by this new symbol.
  //
  // Complexity: O(lgN) where N is the # of symbols in current scope
  //
  
  void insert(KeyT key, SymbolT symbol)
  {
	//If true, the key does exist in this current scope, and we can set the key to have a new symbol value.
	//The reason we distinguish between whether or not it exists is because some weird things can happen
	//if you set a nonexistent key to have a certain value...
	
	if(symboltable.front().Symbols.count(key)>0){
		symboltable.front().Symbols[key] = symbol;
	}
	// else we emplace the key (which does not exist in this current scope) into this current scope.
	else{
		symboltable.front().Symbols.emplace(key,symbol);
	}
  }

  //
  // lookup
  //
  // Searches the symbol table for the first (key, symbol) pair that 
  // matches the given key.  The search starts in the current scope, and 
  // proceeds "outward" to the GLOBAL scope.  If a matching (key, symbol)
  // pair is found, true is returned along with a copy of the symbol (via 
  // "symbol" reference parameter).  If not found, false is returned and
  // the "symbol" parameter is left unchanged.
  //
  // NOTE: the search can be controlled by the "option" parameter.  By 
  // default, the entire symbol table is searched as described above.
  // However, the search can also be limited to just the current scope, 
  // or just the GLOBAL scope, via the "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   string                   symbol;
  //   bool                     found;
  //   ...
  //   found = table.lookup("i", 
  //                        symbol, 
  //                        symtable<string,string>::ScopeOption::CURRENT);
  //
  // Complexity: O(SlgN) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  bool lookup(KeyT key, 
              SymbolT& symbol, 
              ScopeOption option = ScopeOption::ALL) 
  {
    
	/* if the scope option is all, then we search all scopes in the deque for the key. We then set symbol to 
	 * that key's value and return true(false otherwise). Oh and because the current scope is at the front of 
	 * the deque. This will get the "nearest" key value as is if it exists. If I didn't add to the front, 
	 * I'd probably need to flip the deque and then search it that way (no thanks) */
	if(option == ScopeOption::ALL){
		for(Scope& s: symboltable){
			if(s.Symbols.find(key) != s.Symbols.end()){
				symbol = s.Symbols[key];
				return true;
			}
		}
	}
	
	/* if the scope option is current, then we search the current scope which is at the front of the deque 
	 * for the key. We then set symbol to be that key's value and return true (false otherwise). 
	 * Note that the current scope would be at the back of the deque if I had added scopes to the back of the deque */
	else if(option == ScopeOption::CURRENT){
		if(symboltable.front().Symbols.find(key) != symboltable.front().Symbols.end()){
			symbol = symboltable.front().Symbols[key];
			return true;
		}
	}
	
	/* if it's not the first two, then the option is global (which is at the back of the deque because we 
	 * add scopes to the front of the deque). We then search the back Scope for the key and set symbol to the
	 * key's value if it exists and return true, false otherwise */
	else{
		if(symboltable.back().Symbols.find(key) != symboltable.back().Symbols.end()){
			symbol = symboltable.back().Symbols[key];
			return true;
		   }
	   }
	   return false;
  } 
  //
  // dump
  // 
  // Dumps the contents of the symbol table to the output stream, 
  // starting with the current scope and working "outward" to the GLOBAL
  // scope.  You can dump the entire symbol table (the default), or dump
  // just the current scope or global scope; this is controlled by the 
  // "option" parameter.
  //
  // Example:
  //   symtable<string,string>  table;
  //   ...
  //   table.dump(std::cout, symtable<string,string>::ScopeOption::GLOBAL);
  //
  // Complexity: O(S*N) where S is the # of scopes and N is the largest #
  // of symbols in any one scope
  //
  void dump(ostream& output, ScopeOption option = ScopeOption::ALL) 
  {
    output << "**************************************************" << endl;
	
	// Need to check if scope is all or current. Otherwise it's global
    if (option == ScopeOption::ALL){
      output << "*************** SYMBOL TABLE (ALL) ***************" << endl;
	  
	  // I don't know why the number of scopes and symbols were at the bottom of this function when
	  // they're outputted right after the symbol table Name, so I put them right after the symbol table name
	  output << "** # of scopes: " << this->numscopes() << endl;
      output << "** # of symbols: " << this->size() << endl;
	  
	  // Get every scope in symboltable and output it's name while also outputting the name and symbols it contains
	  for(Scope& s: symboltable){
		  output << "** " << s.Name << " **" <<endl;
		  
		  for(pair<KeyT, SymbolT> p : s.Symbols){
			  output << p.first << ": " << p.second << endl;
		  }
	  }
	}
	
    else if (option == ScopeOption::CURRENT){
      output << "*************** SYMBOL TABLE (CUR) ***************" << endl;
	  output << "** # of scopes: " << this->numscopes() << endl;
	  output << "** # of symbols: " << this->size() << endl;
	  
	  // Oh look. Why is this if statement here? Because dump can't output a current or global scope if there are no
	  // Scopes. The functions ahead would access things that don't exist and would cause a seg fault, so we check 
	  // that a Scope exists for both Current option and Global option. This outputs the current scope's Name while
	  // also outputting the current Scopes' Symbols' name and symbols
	  if(this-> numscopes() > 0){
		  output << "** " << symboltable.front().Name << " **" <<endl;
		  for(pair<KeyT, SymbolT> p : symboltable.front().Symbols){
			  output << p.first << ": " <<p.second << endl;
		  }
		}
	}
	
    else { // global:
      output << "*************** SYMBOL TABLE (GBL) ***************" << endl;
	  output << "** # of scopes: " << this->numscopes() << endl;
      output << "** # of symbols: " << this->size() << endl;
	  
	  //Explanation for if statement above. This outputs the current scope's Name while 
	  //also outputting the current Scopes' Symbols' name and symbols
	 
	  if(this->numscopes() > 0){
		  output << "** " <<symboltable.back().Name << " **" <<endl;
	  
		  for(pair<KeyT, SymbolT> p : symboltable.back().Symbols){
			  output << p.first << ": " <<p.second << endl;
		  }
	  }
	}

    //
    // output format per scope:
    //
    // ** scopename **
    // key: symbol
    // key: symbol
    // ...
    //
	
	// linnnnnnnnnnnneeeeee....//
    output << "**************************************************" << endl;
  }

};
