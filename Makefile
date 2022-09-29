# Makefile
 
PROGRAM = cls4
OBJS = cls4.o Argument.o Token.o TokenList.o Node.o SyntaxTree.o Object.o Type.o SymbolTable.o Interpreter.o Error.o
COMPILER = g++#-O3

.SUFFIXES: .cpp .o

$(PROGRAM): $(OBJS)
	$(COMPILER) -std=c++17 -o cls4 $^

.cpp.o:
	$(COMPILER) -std=c++17 -c $<

clean:
	rm -f $(PROGRAM) $(OBJS)