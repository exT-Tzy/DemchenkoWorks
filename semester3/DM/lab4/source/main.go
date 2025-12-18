package main

import (
	"bufio"
	"flag"
	"fmt"
	"math"
	"os"
	"strconv"
	"strings"
)

type Graph struct {
	vertex_count int
	adj_matrix   [][]int
	is_directed  bool
}

func main() {
	edges_file := flag.String("e", "", "edges list file path")
	matrix_file := flag.String("m", "", "adjacency matrix file path")
	list_file := flag.String("l", "", "adjacency list file path")
	output_file := flag.String("o", "", "output file path")
	begin_vertex := flag.Int("n", 0, "begin vertex number")
	end_vertex := flag.Int("d", -1, "end vertex number")
	help := flag.Bool("h", false, "show help")

	flag.Parse()

	if *help {
		print_help()
		return
	}

	input_count := 0
	if *edges_file != "" {
		input_count++
	}
	if *matrix_file != "" {
		input_count++
	}
	if *list_file != "" {
		input_count++
	}

	if input_count != 1 {
		fmt.Println("Exactly one input file must be specified")
		return
	}

	var graph Graph
	var err error

	if *edges_file != "" {
		graph, err = read_edges_list(*edges_file)
	} else if *matrix_file != "" {
		graph, err = read_adjacency_matrix(*matrix_file)
	} else {
		graph, err = read_adjacency_list(*list_file)
	}

	if err != nil {
		fmt.Printf("Reading file error: %v\n", err)
		return
	}

	if *end_vertex == -1 {
		*end_vertex = graph.vertex_count - 1
	}

	result := analyze_graph(graph, *begin_vertex, *end_vertex)

	if *output_file != "" {
		err = write_to_file(*output_file, result)
		if err != nil {
			fmt.Printf("Writing to file error: %v\n", err)
			return
		}
	} else {
		fmt.Print(result)
	}
}

func print_help() {
	fmt.Println("Author: Nikita Demchenko")
	fmt.Println("Group: ITPM-124")
	fmt.Println()
	fmt.Println("Usage:")
	fmt.Println("  -e <file>  edges list file path")
	fmt.Println("  -m <file>  adjacency matrix file path")
	fmt.Println("  -l <file>  adjacency list file path")
	fmt.Println("  -o <file>  output file path")
	fmt.Println("  -n <num>   begin vertex number (default: 0)")
	fmt.Println("  -d <num>   end vertex number (default: last vertex)")
	fmt.Println("  -h         show this help")
}

func read_edges_list(filename string) (Graph, error) {
	file, err := os.Open(filename)
	if err != nil {
		return Graph{}, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	vertex_count, _ := strconv.Atoi(strings.TrimSpace(scanner.Text()))

	adj_matrix := make([][]int, vertex_count)
	for i := 0; i < vertex_count; i++ {
		adj_matrix[i] = make([]int, vertex_count)
	}

	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			continue
		}
		parts := strings.Fields(line)
		from, _ := strconv.Atoi(parts[0])
		to, _ := strconv.Atoi(parts[1])
		weight := 1
		if len(parts) > 2 {
			weight, _ = strconv.Atoi(parts[2])
		}
		adj_matrix[from-1][to-1] = weight
	}

	graph := Graph{
		vertex_count: vertex_count,
		adj_matrix:   adj_matrix,
	}
	graph.is_directed = check_if_directed(graph)

	return graph, nil
}

func read_adjacency_matrix(filename string) (Graph, error) {
	file, err := os.Open(filename)
	if err != nil {
		return Graph{}, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	vertex_count, _ := strconv.Atoi(strings.TrimSpace(scanner.Text()))

	adj_matrix := make([][]int, vertex_count)
	for i := 0; i < vertex_count; i++ {
		adj_matrix[i] = make([]int, vertex_count)
		scanner.Scan()
		parts := strings.Fields(scanner.Text())
		for j := 0; j < vertex_count; j++ {
			adj_matrix[i][j], _ = strconv.Atoi(parts[j])
		}
	}

	graph := Graph{
		vertex_count: vertex_count,
		adj_matrix:   adj_matrix,
	}
	graph.is_directed = check_if_directed(graph)

	return graph, nil
}

func read_adjacency_list(filename string) (Graph, error) {
	file, err := os.Open(filename)
	if err != nil {
		return Graph{}, err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	scanner.Scan()
	vertex_count, _ := strconv.Atoi(strings.TrimSpace(scanner.Text()))

	adj_matrix := make([][]int, vertex_count)
	for i := 0; i < vertex_count; i++ {
		adj_matrix[i] = make([]int, vertex_count)
	}

	vertex_idx := 0
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line == "" {
			vertex_idx++
			continue
		}
		parts := strings.Fields(line)
		for _, part := range parts {
			edge_parts := strings.Split(part, ":")
			to, _ := strconv.Atoi(edge_parts[0])
			weight := 1
			if len(edge_parts) > 1 {
				weight, _ = strconv.Atoi(edge_parts[1])
			}
			adj_matrix[vertex_idx][to-1] = weight
		}
		vertex_idx++
	}

	graph := Graph{
		vertex_count: vertex_count,
		adj_matrix:   adj_matrix,
	}
	graph.is_directed = check_if_directed(graph)

	return graph, nil
}

func check_if_directed(graph Graph) bool {
	for i := 0; i < graph.vertex_count; i++ {
		for j := 0; j < graph.vertex_count; j++ {
			if graph.adj_matrix[i][j] != graph.adj_matrix[j][i] {
				return true
			}
		}
	}
	return false
}

func analyze_graph(graph Graph, begin_vertex int, end_vertex int) string {
	var result strings.Builder

	if graph.is_directed {
		result.WriteString("1. Graph is directed.\n\n")
	} else {
		result.WriteString("1. Graph is undirected.\n\n")
	}

	deg_in, deg_out := calculate_degrees(graph)
	result.WriteString("2. deg_in = [")
	for i := 0; i < len(deg_in); i++ {
		if i > 0 {
			result.WriteString(", ")
		}
		result.WriteString(strconv.Itoa(deg_in[i]))
	}
	result.WriteString("]\n   deg_out = [")
	for i := 0; i < len(deg_out); i++ {
		if i > 0 {
			result.WriteString(", ")
		}
		result.WriteString(strconv.Itoa(deg_out[i]))
	}
	result.WriteString("].\n\n")

	if graph.is_directed {
		weak_components := find_weakly_connected_components(graph)
		if len(weak_components) > 1 {
			result.WriteString("Digraph is not connected\n")
		} else {
			result.WriteString("Digraph is connected\n")
		}
		result.WriteString("4. Weakly connected components:\n")
		for _, comp := range weak_components {
			result.WriteString("[")
			for i, v := range comp {
				if i > 0 {
					result.WriteString(", ")
				}
				result.WriteString(strconv.Itoa(v + 1))
			}
			result.WriteString("]\n")
		}
		result.WriteString("\n")

		strong_components := find_strongly_connected_components(graph)
		result.WriteString("5. Strongly connected components:\n")
		for _, comp := range strong_components {
			result.WriteString("[")
			for i, v := range comp {
				if i > 0 {
					result.WriteString(", ")
				}
				result.WriteString(strconv.Itoa(v + 1))
			}
			result.WriteString("]\n")
		}
		result.WriteString("\n")
	} else {
		components := find_connected_components(graph)
		if len(components) > 1 {
			result.WriteString("Graph is not connected\n")
		} else {
			result.WriteString("Graph is connected\n")
		}
		result.WriteString("3. Connected components:\n")
		for _, comp := range components {
			result.WriteString("[")
			for i, v := range comp {
				if i > 0 {
					result.WriteString(", ")
				}
				result.WriteString(strconv.Itoa(v + 1))
			}
			result.WriteString("]\n")
		}
		result.WriteString("\n")

		diameter, radius, center, periphery := calculate_graph_characteristics(graph)
		result.WriteString("6. Diameter: ")
		if diameter == math.MaxInt32 {
			result.WriteString("+Infinity\n")
		} else {
			result.WriteString(strconv.Itoa(diameter) + "\n")
		}
		result.WriteString("   Radius: ")
		if radius == math.MaxInt32 {
			result.WriteString("+Infinity\n")
		} else {
			result.WriteString(strconv.Itoa(radius) + "\n")
		}
		result.WriteString("   Center vertices: [")
		for i, v := range center {
			if i > 0 {
				result.WriteString(", ")
			}
			result.WriteString(strconv.Itoa(v + 1))
		}
		result.WriteString("]\n")
		result.WriteString("   Periphery vertices: [")
		for i, v := range periphery {
			if i > 0 {
				result.WriteString(", ")
			}
			result.WriteString(strconv.Itoa(v + 1))
		}
		result.WriteString("]\n\n")
	}

	result.WriteString("7. Distancies and shortest paths.\n")
	distance, path := dijkstra(graph, begin_vertex, end_vertex)
	result.WriteString(fmt.Sprintf("from %d to %d\n", begin_vertex+1, end_vertex+1))
	if distance == math.MaxInt32 {
		result.WriteString("no path (+Infinity)\n")
	} else {
		result.WriteString(fmt.Sprintf("distance: %d\n", distance))
		result.WriteString("path: ")
		for i, v := range path {
			if i > 0 {
				result.WriteString(" -> ")
			}
			result.WriteString(strconv.Itoa(v + 1))
		}
		result.WriteString("\n")
	}

	return result.String()
}

func calculate_degrees(graph Graph) ([]int, []int) {
	deg_in := make([]int, graph.vertex_count)
	deg_out := make([]int, graph.vertex_count)

	for i := 0; i < graph.vertex_count; i++ {
		for j := 0; j < graph.vertex_count; j++ {
			if graph.adj_matrix[i][j] > 0 {
				deg_out[i]++
				deg_in[j]++
			}
		}
	}

	return deg_in, deg_out
}

func find_connected_components(graph Graph) [][]int {
	visited := make([]bool, graph.vertex_count)
	components := [][]int{}

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] {
			component := []int{}
			dfs_connected(graph, i, visited, &component)
			components = append(components, component)
		}
	}

	return components
}

func dfs_connected(graph Graph, vertex int, visited []bool, component *[]int) {
	visited[vertex] = true
	*component = append(*component, vertex)

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] && (graph.adj_matrix[vertex][i] > 0 || graph.adj_matrix[i][vertex] > 0) {
			dfs_connected(graph, i, visited, component)
		}
	}
}

func find_weakly_connected_components(graph Graph) [][]int {
	visited := make([]bool, graph.vertex_count)
	components := [][]int{}

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] {
			component := []int{}
			dfs_weakly_connected(graph, i, visited, &component)
			components = append(components, component)
		}
	}

	return components
}

func dfs_weakly_connected(graph Graph, vertex int, visited []bool, component *[]int) {
	visited[vertex] = true
	*component = append(*component, vertex)

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] && (graph.adj_matrix[vertex][i] > 0 || graph.adj_matrix[i][vertex] > 0) {
			dfs_weakly_connected(graph, i, visited, component)
		}
	}
}

func find_strongly_connected_components(graph Graph) [][]int {
	stack := []int{}
	visited := make([]bool, graph.vertex_count)

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] {
			dfs_fill_order(graph, i, visited, &stack)
		}
	}

	transposed := transpose_graph(graph)
	visited = make([]bool, graph.vertex_count)
	components := [][]int{}

	for len(stack) > 0 {
		vertex := stack[len(stack)-1]
		stack = stack[:len(stack)-1]

		if !visited[vertex] {
			component := []int{}
			dfs_strongly_connected(transposed, vertex, visited, &component)
			components = append(components, component)
		}
	}

	return components
}

func dfs_fill_order(graph Graph, vertex int, visited []bool, stack *[]int) {
	visited[vertex] = true

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] && graph.adj_matrix[vertex][i] > 0 {
			dfs_fill_order(graph, i, visited, stack)
		}
	}

	*stack = append(*stack, vertex)
}

func transpose_graph(graph Graph) Graph {
	transposed := Graph{
		vertex_count: graph.vertex_count,
		adj_matrix:   make([][]int, graph.vertex_count),
		is_directed:  graph.is_directed,
	}

	for i := 0; i < graph.vertex_count; i++ {
		transposed.adj_matrix[i] = make([]int, graph.vertex_count)
		for j := 0; j < graph.vertex_count; j++ {
			transposed.adj_matrix[i][j] = graph.adj_matrix[j][i]
		}
	}

	return transposed
}

func dfs_strongly_connected(graph Graph, vertex int, visited []bool, component *[]int) {
	visited[vertex] = true
	*component = append(*component, vertex)

	for i := 0; i < graph.vertex_count; i++ {
		if !visited[i] && graph.adj_matrix[vertex][i] > 0 {
			dfs_strongly_connected(graph, i, visited, component)
		}
	}
}

func calculate_graph_characteristics(graph Graph) (int, int, []int, []int) {
	dist_matrix := floyd_warshall(graph)

	eccentricities := make([]int, graph.vertex_count)
	for i := 0; i < graph.vertex_count; i++ {
		max_dist := 0
		for j := 0; j < graph.vertex_count; j++ {
			if i != j && dist_matrix[i][j] != math.MaxInt32 && dist_matrix[i][j] > max_dist {
				max_dist = dist_matrix[i][j]
			}
		}
		if max_dist == 0 {
			eccentricities[i] = math.MaxInt32
		} else {
			eccentricities[i] = max_dist
		}
	}

	radius := math.MaxInt32
	diameter := 0
	for i := 0; i < graph.vertex_count; i++ {
		if eccentricities[i] != math.MaxInt32 {
			if eccentricities[i] < radius {
				radius = eccentricities[i]
			}
			if eccentricities[i] > diameter {
				diameter = eccentricities[i]
			}
		}
	}

	center := []int{}
	periphery := []int{}
	for i := 0; i < graph.vertex_count; i++ {
		if eccentricities[i] == radius {
			center = append(center, i)
		}
		if eccentricities[i] == diameter {
			periphery = append(periphery, i)
		}
	}

	return diameter, radius, center, periphery
}

func floyd_warshall(graph Graph) [][]int {
	dist := make([][]int, graph.vertex_count)
	for i := 0; i < graph.vertex_count; i++ {
		dist[i] = make([]int, graph.vertex_count)
		for j := 0; j < graph.vertex_count; j++ {
			if i == j {
				dist[i][j] = 0
			} else if graph.adj_matrix[i][j] > 0 {
				dist[i][j] = graph.adj_matrix[i][j]
			} else {
				dist[i][j] = math.MaxInt32
			}
		}
	}

	for k := 0; k < graph.vertex_count; k++ {
		for i := 0; i < graph.vertex_count; i++ {
			for j := 0; j < graph.vertex_count; j++ {
				if dist[i][k] != math.MaxInt32 && dist[k][j] != math.MaxInt32 {
					if dist[i][k]+dist[k][j] < dist[i][j] {
						dist[i][j] = dist[i][k] + dist[k][j]
					}
				}
			}
		}
	}

	return dist
}

func dijkstra(graph Graph, start int, end int) (int, []int) {
	dist := make([]int, graph.vertex_count)
	visited := make([]bool, graph.vertex_count)
	prev := make([]int, graph.vertex_count)

	for i := 0; i < graph.vertex_count; i++ {
		dist[i] = math.MaxInt32
		prev[i] = -1
	}
	dist[start] = 0

	for i := 0; i < graph.vertex_count; i++ {
		min_dist := math.MaxInt32
		min_vertex := -1

		for j := 0; j < graph.vertex_count; j++ {
			if !visited[j] && dist[j] < min_dist {
				min_dist = dist[j]
				min_vertex = j
			}
		}

		if min_vertex == -1 {
			break
		}

		visited[min_vertex] = true

		for j := 0; j < graph.vertex_count; j++ {
			if graph.adj_matrix[min_vertex][j] > 0 {
				new_dist := dist[min_vertex] + graph.adj_matrix[min_vertex][j]
				if new_dist < dist[j] {
					dist[j] = new_dist
					prev[j] = min_vertex
				}
			}
		}
	}

	path := []int{}
	if dist[end] != math.MaxInt32 {
		current := end
		for current != -1 {
			path = append([]int{current}, path...)
			current = prev[current]
		}
	}

	return dist[end], path
}

func write_to_file(filename string, content string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	_, err = file.WriteString(content)
	return err
}
