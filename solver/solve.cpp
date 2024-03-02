#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <filesystem>

#include <cstdio>
#include <cstdlib>

#include "bottom-left-heuristic-master/main.h"
// #include "bottom-left-heuristic-master/globals.h"

using namespace std;

// Verify current OS
enum {WINDOWS_S, UNIX_S};
#if defined(__WIN32) || defined(_WIN64)
	int idOS_SOL = WINDOWS_S;
	char* string_endline_SOL = new char[2];
#endif

#ifdef __unix__
	int idOS_SOL = UNIX_S;
	char* string_endline_SOL = new char[1];
#endif


// make filesystem usable with c++17 with 'fs'
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
    #if __cplusplus >= 201703L && __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
    #elif __has_include(<experimental/filesystem>)
        #include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
    #elif __has_include(<boost/filesystem.hpp>)
        #include <boost/filesystem.hpp>
        namespace fs = boost::filesystem;
    #endif
#endif

// enum {SQUARE, RECTANGLE, TRIANGLE, CIRCLE};
// RECTANGLE, TRIANGLE, CIRCLE, are defined on the globals.h .
enum {SQUARE};

const char* lable_circle = "Círculo";
const char* lable_square = "Quadrado";
const char* lable_rectangle = "Retângulo";
const char* lable_triangle = "Triângulo";

const char* solver_input_name = "solver_input.txt";
fs::path solver_input_path = "solver_input.txt";

// Represents a piece from the pieces' file
struct Piece {
	int piece_type;
	char* description;
	long double height;
	long double width;
	long double length;
	int amount;
};


// Execute a system command
int exec_command_line(const char* command) {
	FILE *fpipe;

	char line[2048];

	cout << string_endline_SOL << command << string_endline_SOL;
	#if defined(_WIN32) || defined(_WIN64)
		fpipe = (FILE*) _popen(command, "r");
	#else
		cout << string_endline_SOL << command << string_endline_SOL;
		fpipe = (FILE*) popen(command,"r");
	#endif
	cout << fpipe << string_endline_SOL;
	// If fpipe is NULL
	if (fpipe == NULL) {  
        cerr << "Problems with pipe" << string_endline_SOL;
		exit(1);
	}

	while (fgets(line, sizeof(line), fpipe) != NULL) {
		cout << line << string_endline_SOL;
	}
	#if defined(_WIN32) || defined(_WIN64)
		return _pclose(fpipe);
	#else
		return pclose(fpipe);
	#endif
}


void convert_tex_solution_to_pdf(const char* solution_file_name, const char* program_root_dir) {
	cout << "Iniciando conversão para PDF" << string_endline_SOL;
	cout << program_root_dir << string_endline_SOL;
	
	// Uses pdflatex to convert the soolution TEX file to a PDF file
	char command[1024] = "\0";
	strcat(command, "pdflatex");
	strcat(command, " ");
	// strcat(command, "--interaction=batchmode");
	strcat(command, "--interaction=nonstopmode");
	strcat(command, " ");
	strcat(command, "-output-directory=");
	strcat(command, program_root_dir);
	strcat(command, " ");
	strcat(command, solution_file_name);

	cout << command << string_endline_SOL;

	exec_command_line(command);
	
	
	cout << "Apagando arquivos auxiliares para geração do PDF" << string_endline_SOL;

	fs::path sol_pdf = solution_file_name;

	cout << sol_pdf.string() << string_endline_SOL;
	cout << fs::exists(sol_pdf) << string_endline_SOL;

	fs::path dir = program_root_dir;
	
	// ------------------------------------------------------
	// Removes extra files that were generated before the pdf
	// ------------------------------------------------------

	// .aux files
	dir.append("*.aux");
	char aux_files[512];
	strcpy(aux_files, dir.string().c_str());
	// strcat(aux_files, "*.aux");
	
	dir = dir.parent_path();

	// .log files
	dir.append("*.log");	
	char log_files[512];
	strcpy(log_files, dir.string().c_str());
	// strcat(log_files, "*.log");
	
	dir = dir.parent_path();
	
	// .synctex.gz files
	dir.append("*.synctex.gz");
	char synctexgz_files[512];
	strcpy(synctexgz_files, dir.string().c_str());
	// strcat(synctexgz_files, "*.synctex.gz");
	
	dir = dir.parent_path();
	
	// .tex files
	dir.append("*.tex");	
	char tex_files[512];
	strcpy(tex_files, dir.string().c_str());
	// strcat(tex_files, "*.tex");

	dir = dir.parent_path();

	strcpy(command, "\0");
	if (idOS_SOL == UNIX_S) {
		// Remove the files on linux

		strcat(command, "rm -f");
		strcat(command, " ");
		strcat(command, aux_files);
		strcat(command, " ");
		strcat(command, log_files);
		strcat(command, " ");
		strcat(command, synctexgz_files);
		strcat(command, " ");
		strcat(command, tex_files);
	}
	else if (idOS_SOL == WINDOWS_S) {
		// Remove the files on windows

		// rmdir /S /Q "diretorio"
		// del "arquivo"
		strcat(command, "del ");
		strcat(command, " ");
		strcat(command, aux_files);
		strcat(command, " ");
		strcat(command, log_files);
		strcat(command, " ");
		strcat(command, synctexgz_files);
		strcat(command, " ");
		strcat(command, tex_files);
	}

	exec_command_line(command);
	cout << "OK" << string_endline_SOL;
}

// Execute Bottom-Left heuristic
void exec_heuristic(const char* solver_path, const char* kiln_file_name, const char* solution_file_name) {	

	
	cout << "Caminhos: " << string_endline_SOL;
	cout << kiln_file_name << string_endline_SOL;
	cout << solver_input_path.string().c_str() << string_endline_SOL;
	cout << solution_file_name << string_endline_SOL;

	cout << "Preparando parâmetros" << string_endline_SOL;
	
	// Create function parameters
	int argc = 4;
	char** argv = new char*[argc];
	for (int i = 0; i < argc; i++) {
		argv[i] = new char[1024];
	}
	strcpy(argv[0], solver_path);
	strcpy(argv[1], kiln_file_name);
	strcpy(argv[2], solver_input_path.string().c_str());
	strcpy(argv[3], solution_file_name);


	// Call heuristic function
	cout << "Iniciando chamada de função" << string_endline_SOL;
	executeHeuristicBottomLeft(argc, argv);
	cout << "Solucao finalizada" << string_endline_SOL;

	// Free parameters memory
	cout << "Liberando Memória usada pelos parâmetros" << string_endline_SOL;
	for (int i = 0; i < argc; i++) {
		delete[] argv[i];
	}
	delete[] argv;
	cout << "OK" << string_endline_SOL;
}

// Delete created input files
void remove_solver_input_file(const char* solver_input_name) {
	cout << "Apagando arquivos auxiliares de execução" << string_endline_SOL;
	char command[512] = "\0";
	if (idOS_SOL == UNIX_S) {
		strcat(command, "rm -f");
		strcat(command, " ");
		strcat(command, solver_input_name);
	}
	else if(idOS_SOL == WINDOWS_S) {
		strcat(command, "del ");
		strcat(command, " ");
		strcat(command, solver_input_name);
	}

	exec_command_line(command);
	cout << "OK" << string_endline_SOL;

}

// Create a pointer to a piece
Piece* create_piece(int in_type, string description, long double height, long double width, long double length, int amount) {
	Piece* piece = new Piece;
	switch (in_type) {    
		case SQUARE:
			piece->piece_type = RECTANGLE;
			piece->length = width;
			break;
		case RECTANGLE:
			piece->piece_type = RECTANGLE;
			piece->length = length;
			break;
		case TRIANGLE:
			piece->piece_type = TRIANGLE;
			break;
		case CIRCLE:
			piece->piece_type = CIRCLE;
			break;
		default:
			break;
	}

	piece->description = new char[description.length() + 1];
	strcpy(piece->description, description.c_str());

	piece->height = height;
	piece->width = width;
	piece->amount = amount;

	return piece;
}

// Free piece memory
void destroy_piece(Piece* piece) {
	delete[] piece->description;
	delete piece;
}

// Get the index of the object type. Returns -1 if there was no lable for the type.
int get_type_code(string lable) {

	if (lable.compare(lable_circle) == 0) {
		return CIRCLE;
	}

	if (lable.compare(lable_square) == 0) {
		return SQUARE;
	}
	

	if (lable.compare(lable_rectangle) == 0) {
		return RECTANGLE;
	}

	if (lable.compare(lable_triangle) == 0) {
		return TRIANGLE;
	}

	return -1;

}


int main(int argc, char *argv[]) {

	if (idOS_SOL == WINDOWS_S) {
		strcpy(string_endline_SOL, "\r\n");
	}
	else if (idOS_SOL == UNIX_S) {
		strcpy(string_endline_SOL, "\n");
	}

	if (argc < 4) {
		cout << "The names for the pieces input file, kiln "
			 << "input file and solution file are needed." 
			 << string_endline_SOL;
	}
	
	// Get input files
	char* pieces_file_name = argv[1];
	char* kiln_file_name = argv[2];
	char* solution_file_name = argv[3];

	cout << "Preparando arquivos de entrada para execucao" << string_endline_SOL;

	// Open pieces files
	ifstream file_pieces;
	file_pieces.open(pieces_file_name);

	// Read the pieces and stores in pieces vector
	vector<Piece*> pieces;
	int number_of_pieces = 0;
	while (file_pieces.good()) {
		string type_str;
		// Read type and verify if it is valid
		file_pieces >> type_str;
		int piece_type = get_type_code(type_str);
		if (piece_type == -1) {
			continue;
		}

		// Read the description
		string description_str;
		file_pieces >> description_str;
		
		// Read the height
		string height_str;
		file_pieces >> height_str;;
		long double height = stod(height_str);

		// Read the width
		string width_str;
		file_pieces >> width_str;
		long double width = stod(width_str);

		// If the piece is a circle, the input was the diameter
		// Must convert to radio
		if (piece_type == CIRCLE) {
			width = ceil(width/2.0);
		}

		// If the piece was a rectangle, then reads length
		string length_str = "0";
		if (piece_type == RECTANGLE) {
			file_pieces >> length_str;
		}
		long double length = stod(length_str);

		// Reads the amount
		string amount_str;
		file_pieces >> amount_str;
		int amount = stoi(amount_str);
		
		// If the amount is more than 0, create piece and stores it on vector
		if (amount > 0) {
			pieces.push_back(create_piece(
				piece_type,
				description_str,
				height,
				width,
				length,
				amount
			));
			number_of_pieces += amount;
		}
	}

	file_pieces.close();

	cout << "OK" << string_endline_SOL;

	cout << "Montando caminhos dos arquivos" << string_endline_SOL;

	fs::path p = argv[0];
	fs::path program_solver_dir = p.parent_path();

	// Create inputs paths
	solver_input_path = program_solver_dir.string().c_str();
	solver_input_path.append(solver_input_name);

	// Create pieces input file
	ofstream file_instance;
	file_instance.open(solver_input_path.string().c_str());

	// Write number of pieces
	file_instance << number_of_pieces << string_endline_SOL;

	// Write each piece
	for (int i = 0; i < (int) pieces.size(); i++) {
		Piece* piece = pieces[i];
		// The solver does not uses the amount
		// Pieces with amount greater than 1 must be repeated
		for (int j = 0; j < piece->amount; j++) {
			file_instance << 0 << " ";
			file_instance << piece->piece_type << " ";
			file_instance << 1 << " ";
			
			file_instance << piece->width << " ";
			if (piece->piece_type == RECTANGLE) {
				file_instance << piece->length << " ";
			}
			file_instance << piece->height << " ";
			
			file_instance << piece->description << string_endline_SOL;
		}
	}

	file_instance.close();

	// Free memory
	for (int i = 0; i < (int) pieces.size(); i++) {
		Piece* piece = pieces[i];
		destroy_piece(piece);
	}

	
	// Create path to solver main file, which should be named "main" ("main.exe" for windows) 
	fs::path exec_path = program_solver_dir.string().c_str();
	exec_path.append("bottom-left-heuristic-master");
	if (idOS_SOL == UNIX_S) {
		exec_path.append("main");
	} 
	else if (idOS_SOL == WINDOWS_S) {
		exec_path.append("main.exe");
	}

	cout << "OK" << string_endline_SOL;	

	// Call solver function
	exec_heuristic(
		exec_path.string().c_str(), 
		kiln_file_name, 
		solution_file_name
	);

	// Create PDF solution
	convert_tex_solution_to_pdf(solution_file_name, program_solver_dir.string().c_str());

	// Delete input file
	remove_solver_input_file(solver_input_path.string().c_str());

	// Free memory
	delete[] string_endline_SOL;
	return 0;
}