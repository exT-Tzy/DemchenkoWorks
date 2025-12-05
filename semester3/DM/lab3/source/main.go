package main

import (
	"fmt"
	"os"
	"regexp"
	"sort"
	"strings"
)

type Token struct {
	Type  string
	Value string
}

type Node struct {
	Type     string
	Value    string
	Left     *Node
	Right    *Node
	Variable string
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: program <filename>")

		return
	}

	data, err := os.ReadFile(os.Args[1])
	if err != nil {
		fmt.Printf("Error reading file: %v\n", err)

		return
	}

	formula := strings.TrimSpace(string(data))
	fmt.Printf("Input formula: %s\n\n", formula)

	tokens, err := tokenize(formula)
	if err != nil {
		fmt.Printf("Error: %v\n", err)

		return
	}

	ast, err := parse(tokens)
	if err != nil {
		fmt.Printf("Error: %v\n", err)

		return
	}

	variables := extractVariables(ast)
	sort.Strings(variables)

	fmt.Println("Truth table:")
	truthTable := buildTruthTable(ast, variables)
	printTruthTable(truthTable, variables)

	fictitious, essential := findFictitiousVariables(truthTable, variables)

	fmt.Printf("\nEssential variables: %v\n", essential)
	fmt.Printf("Fictitious variables: %v\n", fictitious)

	if len(fictitious) > 0 {
		for _, fv := range fictitious {
			variables = removeVariable(variables, fv)
		}

		ast = removeFictitiousFromAST(ast, fictitious)
		truthTable = buildTruthTable(ast, variables)

		fmt.Println("\nTruth table after removing fictitious variables:")
		printTruthTable(truthTable, variables)
	}

	dual := buildDualFunction(truthTable)
	fmt.Println("\nDual function truth table:")
	printTruthTable(dual, variables)

	sdnf := buildSDNF(truthTable, variables)
	fmt.Printf("\nPrincipal Disjunctive Normal Form (SDNF):\n%s\n", sdnf)

	sknf := buildSKNF(truthTable, variables)
	fmt.Printf("\nPrincipal Conjunctive Normal Form (SKNF):\n%s\n", sknf)

	anf := buildANF(truthTable, variables)
	fmt.Printf("\nAlgebraic Normal Form (ANF):\n%s\n", anf)

	dualSDNF := buildSDNF(dual, variables)
	fmt.Printf("\nSDNF of dual function:\n%s\n", dualSDNF)

	dualSKNF := buildSKNF(dual, variables)
	fmt.Printf("\nSKNF of dual function:\n%s\n", dualSKNF)
}

func tokenize(formula string) ([]Token, error) {
	var tokens []Token
	varPattern := regexp.MustCompile(`^([a-z]_\d+)`)

	i := 0
	for i < len(formula) {
		ch := formula[i]

		if ch == ' ' || ch == '\t' || ch == '\n' {
			i++

			continue
		}

		if ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' {
			tokens = append(tokens, Token{Type: "BRACKET", Value: string(ch)})
			i++

			continue
		}

		if ch == '+' || ch == '&' || ch == '@' || ch == '~' || ch == '>' || ch == '|' || ch == '!' {
			tokens = append(tokens, Token{Type: "OP", Value: string(ch)})
			i++

			continue
		}

		if ch == '-' {
			tokens = append(tokens, Token{Type: "NOT", Value: "-"})
			i++

			continue
		}

		if ch >= 'a' && ch <= 'z' {
			match := varPattern.FindString(formula[i:])
			if match != "" {
				tokens = append(tokens, Token{Type: "VAR", Value: match})
				i += len(match)

				continue
			}
		}

		return nil, fmt.Errorf("invalid character at position %d: %c", i, ch)
	}

	return tokens, nil
}

func parse(tokens []Token) (*Node, error) {
	pos := 0

	var parseExpr func() (*Node, error)

	parseExpr = func() (*Node, error) {
		if pos >= len(tokens) {
			return nil, fmt.Errorf("unexpected end of expression")
		}

		token := tokens[pos]

		if token.Type == "NOT" {
			pos++

			operand, err := parseExpr()
			if err != nil {
				return nil, err
			}

			return &Node{Type: "NOT", Left: operand}, nil
		}

		if token.Type == "VAR" {
			pos++

			return &Node{Type: "VAR", Variable: token.Value}, nil
		}

		if token.Type == "BRACKET" && (token.Value == "(" || token.Value == "{" || token.Value == "[") {
			openBracket := token.Value
			pos++

			left, err := parseExpr()
			if err != nil {
				return nil, err
			}

			if pos >= len(tokens) {
				return nil, fmt.Errorf("expected closing bracket")
			}

			if tokens[pos].Type == "BRACKET" {
				closeBracket := tokens[pos].Value
				if !matchBracket(openBracket, closeBracket) {
					return nil, fmt.Errorf("mismatched brackets: %s and %s", openBracket, closeBracket)
				}

				pos++

				return left, nil
			}

			if tokens[pos].Type != "OP" {
				return nil, fmt.Errorf("expected binary operator or closing bracket")
			}

			op := tokens[pos].Value
			pos++

			right, err := parseExpr()
			if err != nil {
				return nil, err
			}

			if pos >= len(tokens) || tokens[pos].Type != "BRACKET" {
				return nil, fmt.Errorf("expected closing bracket")
			}

			closeBracket := tokens[pos].Value
			if !matchBracket(openBracket, closeBracket) {
				return nil, fmt.Errorf("mismatched brackets: %s and %s", openBracket, closeBracket)
			}
			pos++

			return &Node{Type: "OP", Value: op, Left: left, Right: right}, nil
		}

		return nil, fmt.Errorf("unexpected token: %v", token)
	}

	result, err := parseExpr()
	if err != nil {
		return nil, err
	}

	if pos < len(tokens) {
		return nil, fmt.Errorf("unexpected tokens after expression")
	}

	return result, nil
}

func matchBracket(open, close string) bool {
	return (open == "(" && close == ")") ||
		(open == "{" && close == "}") ||
		(open == "[" && close == "]")
}

func extractVariables(node *Node) []string {
	varMap := make(map[string]bool)
	var traverse func(*Node)

	traverse = func(n *Node) {
		if n == nil {
			return
		}

		if n.Type == "VAR" {
			varMap[n.Variable] = true
		}

		traverse(n.Left)
		traverse(n.Right)
	}

	traverse(node)

	vars := make([]string, 0, len(varMap))
	for v := range varMap {
		vars = append(vars, v)
	}

	return vars
}

func buildTruthTable(ast *Node, variables []string) []bool {
	n := len(variables)
	size := 1 << n
	table := make([]bool, size)

	for i := 0; i < size; i++ {
		values := make(map[string]bool)
		for j, v := range variables {
			values[v] = (i & (1 << (n - 1 - j))) != 0
		}

		table[i] = evaluate(ast, values)
	}

	return table
}

func evaluate(node *Node, values map[string]bool) bool {
	if node.Type == "VAR" {
		return values[node.Variable]
	}

	if node.Type == "NOT" {
		return !evaluate(node.Left, values)
	}

	left := evaluate(node.Left, values)
	right := evaluate(node.Right, values)

	switch node.Value {
	case "+":
		return left || right

	case "&":
		return left && right

	case "@":
		return left != right

	case "~":
		return left == right

	case ">":
		return !left || right

	case "|":
		return !(left && right)

	case "!":
		return !(left || right)
	}

	return false
}

func printTruthTable(table []bool, variables []string) {
	n := len(variables)

	for _, v := range variables {
		fmt.Printf("%s\t", v)
	}
	fmt.Println("F")

	for i := 0; i < len(table); i++ {
		for j := 0; j < n; j++ {
			if (i & (1 << (n - 1 - j))) != 0 {
				fmt.Print("1\t")
			} else {
				fmt.Print("0\t")
			}
		}
		if table[i] {
			fmt.Println("1")
		} else {
			fmt.Println("0")
		}
	}
}

func findFictitiousVariables(table []bool, variables []string) ([]string, []string) {
	n := len(variables)
	fictitious := []string{}
	essential := []string{}

	for i, v := range variables {
		isFictitious := true
		size := len(table)

		for j := 0; j < size; j++ {
			if (j & (1 << (n - 1 - i))) == 0 {
				flipped := j | (1 << (n - 1 - i))
				if table[j] != table[flipped] {
					isFictitious = false

					break
				}
			}
		}

		if isFictitious {
			fictitious = append(fictitious, v)
		} else {
			essential = append(essential, v)
		}
	}

	return fictitious, essential
}

func removeVariable(variables []string, toRemove string) []string {
	result := []string{}
	for _, v := range variables {
		if v != toRemove {
			result = append(result, v)
		}
	}

	return result
}

func removeFictitiousFromAST(node *Node, fictitious []string) *Node {
	if node == nil {
		return nil
	}

	if node.Type == "VAR" {
		for _, f := range fictitious {
			if node.Variable == f {
				return &Node{Type: "VAR", Variable: "TRUE"}
			}
		}

		return node
	}

	node.Left = removeFictitiousFromAST(node.Left, fictitious)
	node.Right = removeFictitiousFromAST(node.Right, fictitious)

	return node
}

func buildDualFunction(table []bool) []bool {
	dual := make([]bool, len(table))
	for i := range table {
		dual[i] = !table[len(table)-1-i]
	}

	return dual
}

func buildSDNF(table []bool, variables []string) string {
	terms := []string{}
	n := len(variables)

	for i := 0; i < len(table); i++ {
		if table[i] {
			term := ""
			for j := 0; j < n; j++ {
				if j > 0 {
					term += " & "
				}

				if (i & (1 << (n - 1 - j))) != 0 {
					term += variables[j]
				} else {
					term += "-" + variables[j]
				}
			}

			terms = append(terms, "("+term+")")
		}
	}

	if len(terms) == 0 {
		return "0"
	}

	return strings.Join(terms, " + ")
}

func buildSKNF(table []bool, variables []string) string {
	terms := []string{}
	n := len(variables)

	for i := 0; i < len(table); i++ {
		if !table[i] {
			term := ""
			for j := 0; j < n; j++ {
				if j > 0 {
					term += " + "
				}

				if (i & (1 << (n - 1 - j))) != 0 {
					term += "-" + variables[j]
				} else {
					term += variables[j]
				}
			}

			terms = append(terms, "("+term+")")
		}
	}

	if len(terms) == 0 {
		return "1"
	}

	return strings.Join(terms, " & ")
}

func buildANF(table []bool, variables []string) string {
	n := len(variables)
	size := len(table)

	coeffs := make([]bool, size)
	copy(coeffs, table)

	for i := 0; i < n; i++ {
		step := 1 << (n - 1 - i)
		for j := 0; j < size; j++ {
			if (j & step) != 0 {
				coeffs[j] = coeffs[j] != coeffs[j^step]
			}
		}
	}

	terms := []string{}
	for i := 0; i < size; i++ {
		if coeffs[i] {
			if i == 0 {
				terms = append(terms, "1")
			} else {
				term := ""
				first := true
				for j := 0; j < n; j++ {
					if (i & (1 << (n - 1 - j))) != 0 {
						if !first {
							term += " & "
						}

						term += variables[j]
						first = false
					}
				}

				terms = append(terms, term)
			}
		}
	}

	if len(terms) == 0 {
		return "0"
	}

	return strings.Join(terms, " @ ")
}
