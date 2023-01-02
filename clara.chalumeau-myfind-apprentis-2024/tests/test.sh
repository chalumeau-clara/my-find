#!/bin/bash


#####################
echo -n "Simple: "
#####################

find . | tr ' ' '\n' > expected
./myfind .| tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind . > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -print: "
#####################

find . -print  | tr ' ' '\n' > expected
./myfind . -print | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind . -print > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -print ast_evaluation/: "
#####################

find ast_evaluation/ -print -print  | tr ' ' '\n' > expected
./myfind ast_evaluation/ -print -print | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -print -print > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -name ast_evaluation/: "
#####################

find ast_evaluation/ -name "ast*"  | tr ' ' '\n' > expected
./myfind ast_evaluation/ -name "ast*" | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -name "ast*" > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -type f ast_evaluation/: "
#####################

find ast_evaluation/ -type f  | tr ' ' '\n' > expected
./myfind ast_evaluation/ -type f | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -type f > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -name \"ast*\" -o -name \"mem*\" ast_evaluation/: "
#####################

find ast_evaluation/ -name "ast*" -o -name "mem*"   | tr ' ' '\n' > expected
./myfind ast_evaluation/ -name "ast*" -o -name "mem*" | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -name "ast*" -o -name "mem*" > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple -name \"ast*\" -a -type f ast_evaluation/: "
#####################

find ast_evaluation/ -name "ast*" -a -type f  | tr ' ' '\n' > expected
./myfind ast_evaluation/ -name "ast*" -a -type f | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -name "ast*" -a -type f > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Medium (priority) -name \"ast*\" -a -type f ast_evaluation/ -o -name \"mem*\": "
#####################

find ast_evaluation/ -name "ast*" -a -type f -o -name "mem*"   | tr ' ' '\n' > expected
./myfind ast_evaluation/ -name "ast*" -a -type f -o -name "mem*" | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -name "ast*" -a -type f -o -name "mem*" > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Medium (priority) -name \"ast*\" -o -type f ast_evaluation/ -a -name \"mem*\": "
#####################

find ast_evaluation/ -name "ast*" -o -type f -a -name "mem*"   | tr ' ' '\n' > expected
./myfind ast_evaluation/ -name "ast*" -o -type f -a -name "mem*" | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/ -name "ast*" -o -type f -a -name "mem*" > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -newer ast_evaluation/foo/bar : "
#####################

find ast_evaluation/foo -newer ast_evaluation/foo/bar  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -newer ast_evaluation/foo/bar | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -newer ast_evaluation/foo/bar > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -newer ast_evaluation/foo/baz : "
#####################

find ast_evaluation/foo -newer ast_evaluation/foo/baz | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -newer ast_evaluation/foo/baz | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -newer ast_evaluation/foo/baz > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo ! -name bar : "
#####################

find ast_evaluation/foo ! -name bar  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo ! -name bar | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo ! -name bar > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple parenthesis ast_evaluation/foo  ( -name bar -o -name baz ): "
#####################

find ast_evaluation/foo \( -name bar -o -name baz \)  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo \( -name bar -o -name baz \) | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo \( -name bar -o -name baz \) > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Medium parenthesis ast_evaluation/foo  ! ( -name bar -o -name baz ): "
#####################

find ast_evaluation/foo ! \( -name bar -o -name baz \)  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo ! \( -name bar -o -name baz \) | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo ! \( -name bar -o -name baz \) > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm 664: "
#####################

find ast_evaluation/foo/bah -perm 664  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm 664 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm 664 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm 660: "
#####################

find ast_evaluation/foo/bah -perm 660  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm 660 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm 660 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm -640: "
#####################

find ast_evaluation/foo/bah -perm -640  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm -640 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm -640 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm -123: "
#####################

find ast_evaluation/foo/bah -perm -123  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm -123 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm -123 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm /640: "
#####################

find ast_evaluation/foo/bah -perm /640  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm /640 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm /640 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple perm ast_evaluation/foo/bah -perm -123: "
#####################

find ast_evaluation/foo/bah -perm /123  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo/bah -perm /123 | tr ' ' '\n' > actual
diff -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo/bah -perm /123 > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -user clara : "
#####################

find ast_evaluation/foo -user clara  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -user clara| tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -user clara > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -user henri : "
#####################

find ast_evaluation/foo -user henri  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -user henri | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -user henri > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -group clara : "
#####################

find ast_evaluation/foo -group clara | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -group clara| tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -group clara > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation/foo -group henri : "
#####################

find ast_evaluation/foo -group henri  | tr ' ' '\n' > expected
./myfind ast_evaluation/foo -group henri | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation/foo -group henri > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi


#####################
echo -n "Simple [option] -d : "
#####################

find -d  | tr ' ' '\n' > expected
./myfind -d | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind -d > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple [option] -H tests/qux tests/foo : "
#####################

find -H tests/qux tests/foo  | tr ' ' '\n' > expected
./myfind -H tests/qux tests/foo | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind -H tests/qux tests/foo > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple [option] -H tests/qux/ tests/foo/ : "
#####################

find -H tests/qux/ tests/foo/  | tr ' ' '\n' > expected
./myfind -H tests/qux/ tests/foo/ | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind -H tests/qux/ tests/foo/ > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple [option] -L: "
#####################

find -L   | tr ' ' '\n' > expected
./myfind -L | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind -L  > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple [option] -L tests/qux/ tests/foo/ : "
#####################

find -L tests/qux/ tests/foo/  | tr ' ' '\n' > expected
./myfind -L tests/qux/ tests/foo/ | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind -L tests/qux/ tests/foo/ > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple   -exec pwd \; -exec echo -- {} -- \;"
#####################

find ast_evaluation  -exec pwd \; -exec echo -- {} -- \; | tr ' ' '\n' > expected
./myfind ast_evaluation -exec pwd \; -exec echo -- {} -- \; | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation -exec pwd \; -exec echo -- {} -- \;  > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple : ast_evaluation -exec md5sum {} \; -exec echo ok \;"
#####################

find ast_evaluation -exec md5sum {} \; -exec echo ok \;   | tr ' ' '\n' > expected
./myfind ast_evaluation -exec md5sum {} \; -exec echo ok \; | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind ast_evaluation -exec md5sum {} \; -exec echo ok \;  > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation -execdir pwd \; -execdir echo -- {} -- \;: "
#####################

find  ast_evaluation -execdir pwd \; -execdir echo -- {} -- \;  | tr ' ' '\n' > expected
./myfind ast_evaluation -execdir pwd \; -execdir echo -- {} -- \; | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind  -execdir ast_evaluation pwd \; -execdir echo -- {} -- \;  > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#####################
echo -n "Simple ast_evaluation -exec echo {} \+ : "
#####################

find  ast_evaluation -exec echo {} \+  | tr ' ' '\n' > expected
./myfind ast_evaluation -exec echo {} \+ | tr ' ' '\n' > actual
diff --color -u expected actual
if [ $? -eq 0 ]; then
    echo -e "Test passed"
fi

valgrind --leak-check=yes --error-exitcode=2 ./myfind  -exec echo {} \+  > error 2>&1
if [ $? -eq 2 ]; then
    cat error
fi

#rm tests/actual, tests/expected, tests/error