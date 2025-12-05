package main

import (
	"bufio"
	"fmt"
	"os"
	"sort"
	"strings"
)

type Relation struct {
	elements map[string]bool
	pairs    map[string]map[string]bool
}

func NewRelation() *Relation {
	return &Relation{
		elements: make(map[string]bool),
		pairs:    make(map[string]map[string]bool),
	}
}

func (r *Relation) addPair(a, b string) {
	if r.pairs[a] == nil {
		r.pairs[a] = make(map[string]bool)
	}
	r.pairs[a][b] = true
}

func (r *Relation) hasPair(a, b string) bool {
	if r.pairs[a] == nil {
		return false
	}

	return r.pairs[a][b]
}

func (r *Relation) isReflexive() bool {
	for elem := range r.elements {
		if !r.hasPair(elem, elem) {
			return false
		}
	}

	return true
}

func (r *Relation) isIrreflexive() bool {
	for elem := range r.elements {
		if r.hasPair(elem, elem) {
			return false
		}
	}

	return true
}

func (r *Relation) isSymmetric() bool {
	for a := range r.pairs {
		for b := range r.pairs[a] {
			if !r.hasPair(b, a) {
				return false
			}
		}
	}

	return true
}

func (r *Relation) isAntisymmetric() bool {
	for a := range r.pairs {
		for b := range r.pairs[a] {
			if a != b && r.hasPair(b, a) {
				return false
			}
		}
	}

	return true
}

func (r *Relation) isAsymmetric() bool {
	for a := range r.pairs {
		for b := range r.pairs[a] {
			if r.hasPair(b, a) {
				return false
			}
		}
	}

	return true
}

func (r *Relation) isTransitive() bool {
	for a := range r.pairs {
		for b := range r.pairs[a] {
			for c := range r.pairs[b] {
				if !r.hasPair(a, c) {
					return false
				}
			}
		}
	}

	return true
}

func (r *Relation) isConnected() bool {
	for a := range r.elements {
		for b := range r.elements {
			if a != b && !r.hasPair(a, b) && !r.hasPair(b, a) {
				return false
			}
		}
	}

	return true
}

func (r *Relation) isComplete() bool {
	for a := range r.elements {
		for b := range r.elements {
			if !r.hasPair(a, b) && !r.hasPair(b, a) {
				return false
			}
		}
	}

	return true
}

func (r *Relation) isEquivalence() bool {
	return r.isReflexive() && r.isSymmetric() && r.isTransitive()
}

func (r *Relation) isPartialOrder() bool {
	return r.isReflexive() && r.isAntisymmetric() && r.isTransitive()
}

func (r *Relation) getEquivalenceClasses() [][]string {
	visited := make(map[string]bool)
	var classes [][]string

	elemList := make([]string, 0, len(r.elements))

	for elem := range r.elements {
		elemList = append(elemList, elem)
	}

	sort.Strings(elemList)

	for _, elem := range elemList {
		if visited[elem] {
			continue
		}

		class := []string{}

		for other := range r.elements {
			if r.hasPair(elem, other) {
				class = append(class, other)
				visited[other] = true
			}
		}

		sort.Strings(class)
		classes = append(classes, class)
	}

	return classes
}

func (r *Relation) getMinimalElements() []string {
	var minimal []string

	for a := range r.elements {
		isMinimal := true

		for b := range r.elements {
			if a != b && r.hasPair(a, b) && !r.hasPair(b, a) {
				isMinimal = false

				break
			}
		}

		if isMinimal {
			minimal = append(minimal, a)
		}
	}

	sort.Strings(minimal)

	return minimal
}

func (r *Relation) getMaximalElements() []string {
	var maximal []string

	for a := range r.elements {
		isMaximal := true

		for b := range r.elements {
			if a != b && r.hasPair(b, a) && !r.hasPair(a, b) {
				isMaximal = false

				break
			}
		}

		if isMaximal {
			maximal = append(maximal, a)
		}
	}

	sort.Strings(maximal)

	return maximal
}

func printSign(value bool) string {
	if value {
		return "+"
	}

	return "-"
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: program <filename>")

		return
	}

	filename := os.Args[1]
	file, err := os.Open(filename)

	if err != nil {
		fmt.Printf("Open file error: %v\n", err)
		return
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	relation := NewRelation()

	if scanner.Scan() {
		elements := strings.Fields(scanner.Text())

		for _, elem := range elements {
			relation.elements[elem] = true
		}
	}

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())

		if line == "" {
			continue
		}

		parts := strings.Fields(line)

		if len(parts) == 2 {
			relation.addPair(parts[0], parts[1])
		}
	}

	fmt.Printf("Reflexivity: %s\n", printSign(relation.isReflexive()))
	fmt.Printf("Antireflexivity: %s\n", printSign(relation.isIrreflexive()))
	fmt.Printf("Symmetry: %s\n", printSign(relation.isSymmetric()))
	fmt.Printf("Antisymmetry: %s\n", printSign(relation.isAntisymmetric()))
	fmt.Printf("Asymmetry: %s\n", printSign(relation.isAsymmetric()))
	fmt.Printf("Transitivity: %s\n", printSign(relation.isTransitive()))
	fmt.Printf("Connectivity: %s\n", printSign(relation.isConnected()))
	fmt.Printf("Completeness: %s\n", printSign(relation.isComplete()))

	if relation.isEquivalence() {
		fmt.Println("\nEquivalence relation")
		classes := relation.getEquivalenceClasses()
		fmt.Println("Equivalence classes:")

		for i, class := range classes {
			fmt.Printf("  Class %d: {%s}\n", i+1, strings.Join(class, ", "))
		}

		fmt.Printf("Partition index: %d\n", len(classes))
	}

	if relation.isPartialOrder() {
		fmt.Println("\nOrder relation")

		minimal := relation.getMinimalElements()
		maximal := relation.getMaximalElements()

		fmt.Printf("Minimal elements: {%s}\n", strings.Join(minimal, ", "))
		fmt.Printf("Maximum elements: {%s}\n", strings.Join(maximal, ", "))
	}
}
