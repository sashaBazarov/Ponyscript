# Определяем компилятор
CXX = g++

# Определяем флаги компиляции
CXXFLAGS = -std=c++1 -Wall -Wextra

# Определяем исходные файлы
SRCS = compiler.cpp lexer.cpp utils.cpp headergenerator.cpp libparcer.cpp gccdockparcer.cpp

# Определяем объектные файлы
OBJS = $(SRCS:.cpp=.o)

# Определяем зависимости
DEPS = lexer.h utils.h wordlist.h headergenerator.h libparcer.h gccdockparcer.h

# Определяем цель по умолчанию
all: pony

# Определяем цель сборки
pony: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Создаем объектные файлы из исходных файлов
%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка сгенерированных файлов
clean:
	rm -f $(OBJS) pony
