package main

import (
	"fmt"
	"os"

	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
)

func insertIntVar(file *ast.File, name string, value int) {
	var before, after []ast.Decl

	if len(file.Decls) > 0 {
		hasImport := false
		if genDecl, ok := file.Decls[0].(*ast.GenDecl); ok {
			hasImport = genDecl.Tok == token.IMPORT
		}

		if hasImport {
			before, after = []ast.Decl{file.Decls[0]}, file.Decls[1:]
		} else {
			after = file.Decls
		}
	}

	file.Decls = append(before,
		&ast.GenDecl{
			Tok: token.VAR,
			Specs: []ast.Spec{
				&ast.ValueSpec{
					Names: []*ast.Ident{ast.NewIdent(name)},
					Type:  ast.NewIdent("int"),
					Values: []ast.Expr{
						&ast.BasicLit{
							Kind:  token.INT,
							Value: fmt.Sprintf("%d", value),
						},
					},
				},
			},
		},
	)
	file.Decls = append(file.Decls, after...)
}

func getUniqueVarName() string {
	return "loopIterations" // TODO
}

func findFuncDeclByName(file *ast.File, name string) *ast.FuncDecl {
	for _, decl := range file.Decls {
		if funcDecl, ok := decl.(*ast.FuncDecl); ok {
			if funcDecl.Name.Name == name {
				return funcDecl
			}
		}
	}

	return nil
}

func insertCounterPrinting(file *ast.File, name string) {
	mainFuncDecl := findFuncDeclByName(file, "main")
	if mainFuncDecl == nil {
		// TODO
	}

	mainFuncDecl.Body.List = append(
		mainFuncDecl.Body.List,
		&ast.ExprStmt{
			X: &ast.CallExpr{
				Fun: &ast.SelectorExpr{
					X:   ast.NewIdent("fmt"),
					Sel: ast.NewIdent("Printf"),
				},
				Args: []ast.Expr{
					&ast.BasicLit{
						Kind:  token.STRING,
						Value: "\"Total loop iterations count: %d\\n\"",
					},
					&ast.Ident{
						Name: name,
					},
				},
			},
		},
	)
}

func getVarIncStmt(name string) *ast.IncDecStmt {
	return &ast.IncDecStmt{
		X: &ast.Ident{
			Name: name,
		},
		Tok: token.INC,
	}
}

func addLoopIterationCount(file *ast.File) {
	counterName := getUniqueVarName()
	insertIntVar(file, counterName, 0)
	insertCounterPrinting(file, counterName)

	ast.Inspect(file, func(node ast.Node) bool {
		switch x := node.(type) {
		case *ast.ForStmt:
			x.Body.List = append(x.Body.List, getVarIncStmt(counterName))
		case *ast.RangeStmt:
			x.Body.List = append(x.Body.List, getVarIncStmt(counterName))
		}
		return true
	})
}

func main() {
	if len(os.Args) != 2 {
		fmt.Println("usage: transform <filename.go>")
		return
	}

	fset := token.NewFileSet()
	if file, err := parser.ParseFile(fset, os.Args[1], nil, parser.ParseComments); err == nil {
		addLoopIterationCount(file)

		if format.Node(os.Stdout, fset, file) != nil {
			fmt.Printf("Formatter error: %v\n", err)
		}
	} else {
		fmt.Printf("Errors in %s\n", os.Args[1])
	}
}
