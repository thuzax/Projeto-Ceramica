#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <filesystem>

#include<stdio.h>

using namespace std;


enum {WINDOWS, UNIX};

char* string_endline; 

#if defined(__WIN32) || defined(_WIN64)
	int idOS = WINDOWS;
	string_endline = new char[2];
#endif

#ifdef __unix__
	int idOS = UNIX;
	int idOS = UNIX;	string_endline = new char[1];
#endif



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


enum {SQUARE, RECTANGLE, TRIANGLE, CIRCLE};

const char* lable_circle = "Círculo";
const char* lable_square = "Quadrado";
const char* lable_rectangle = "Retângulo";
const char* lable_triangle = "Triângulo";

const char* solver_input_name = "solver_input.txt";
fs::path solver_input_path = "solver_input.txt";


struct Piece {
	int piece_type;
	char* description;
	double height;
	double width;
	double length;
	int amount;
};

void exec_command_line(const char* command) {
	FILE *fpipe;

	cout << endl << command << endl;

	char line[2048];

	fpipe = (FILE*) popen(command,"r");
	// If fpipe is NULL
	if (fpipe == nullptr) {  
		perror("Problems with pipe");
		exit(1);
	}

	while (fgets(line, sizeof(line), fpipe)) {
		cout << line << endl;
	}
	
	pclose(fpipe);
}


void convert_tex_solution_to_pdf(const char* solution_file_name, const char* program_root_dir) {
	cout << program_root_dir << endl;
	
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

	cout << command << endl;

	exec_command_line(command);
	
	fs::path sol_pdf = solution_file_name;

	cout << sol_pdf.string() << endl;
	cout << fs::exists(sol_pdf) << endl;

	fs::path dir = program_root_dir;

	dir.append("*.aux");
	char aux_files[512];
	strcpy(aux_files, dir.string().c_str());
	// strcat(aux_files, "*.aux");
	
	dir = dir.parent_path();
	
	dir.append("*.log");	
	char log_files[512];
	strcpy(log_files, dir.string().c_str());
	// strcat(log_files, "*.log");
	
	dir = dir.parent_path();
	
	dir.append("*.synctex.gz");
	char synctexgz_files[512];
	strcpy(synctexgz_files, dir.string().c_str());
	// strcat(synctexgz_files, "*.synctex.gz");
	
	dir = dir.parent_path();
	
	dir.append("*.tex");	
	char tex_files[512];
	strcpy(tex_files, dir.string().c_str());
	// strcat(tex_files, "*.tex");

	dir = dir.parent_path();

	strcpy(command, "\0");
	if (idOS == UNIX) {
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
	else if (idOS == WINDOWS) {
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

	// exec_command_line(command);
}


void exec_heuristic(const char* solver_path, const char* kiln_file_name, const char* solution_file_name) {
	char command[512] = "\0";
	strcat(command, solver_path);
	strcat(command, " ");
	strcat(command, kiln_file_name);
	strcat(command, " ");
	strcat(command, solver_input_path.string().c_str());
	strcat(command, " ");
	strcat(command, solution_file_name);
	
	exec_command_line(command);
}

void remove_solver_input_file(const char* solver_input_name) {
	char command[512] = "\0";
	if (idOS == UNIX) {
		strcat(command, "rm -f");
		strcat(command, " ");
		strcat(command, solver_input_name);
	}
	else if(idOS == WINDOWS) {
		strcat(command, "del ");
		strcat(command, " ");
		strcat(command, solver_input_name);
	}

	// exec_command_line(command);

}


Piece* create_piece(int in_type, string description, double height, double width, double length, int amount) {
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

void destroy_piece(Piece* piece) {
	delete[] piece->description;
	delete piece;
}


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

	if (idOS == WINDOWS) {
		strcpy(string_endline, "\r\n");
	}
	else if (idOS == UNIX) {
		strcpy(string_endline, "\n");
	}

	if (argc < 4) {
		cout << "The names for the pieces input file, kiln "
			 << "input file and solution file are needed." 
			 << endl;
	}

	char* pieces_file_name = argv[1];
	char* kiln_file_name = argv[2];
	char* solution_file_name = argv[3];

	ifstream file_pieces;

	file_pieces.open(pieces_file_name);

	vector<Piece*> pieces;

	int number_of_pieces = 0;

	while (file_pieces.good()) {
		string type_str;
		file_pieces >> type_str;

		int piece_type = get_type_code(type_str);

		if (piece_type == -1) {
			continue;
		}

		string description_str;
		file_pieces >> description_str;
		
		string height_str;
		file_pieces >> height_str;;
		double height = stod(height_str);

		string width_str;
		file_pieces >> width_str;
		double width = stod(width_str);

		if (piece_type == CIRCLE) {
			width = ceil(width/2.0);
		}

		string length_str = "0";
		if (piece_type == RECTANGLE) {
			file_pieces >> length_str;
		}
		double length = stod(length_str);

		string amount_str;
		file_pieces >> amount_str;
		int amount = stoi(amount_str);
		
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

	fs::path p = argv[0];
	fs::path program_solver_dir = p.parent_path();

	solver_input_path = program_solver_dir.string().c_str();
	solver_input_path.append(solver_input_name);

	ofstream file_instance;
	file_instance.open(solver_input_path.string().c_str());

	file_instance << number_of_pieces << string_endline;
	for (int i = 0; i < (int) pieces.size(); i++) {
		Piece* piece = pieces[i];
		for (int j = 0; j < piece->amount; j++) {
			file_instance << 0 << " ";
			file_instance << piece->piece_type << " ";
			file_instance << 1 << " ";
			
			file_instance << piece->width << " ";
			if (piece->piece_type == RECTANGLE) {
				file_instance << piece->length << " ";
			}
			file_instance << piece->height << " ";
			
			file_instance << piece->description << string_endline;
		}
	}

	file_instance.close();

	fs::path exec_path = program_solver_dir.string().c_str();
	exec_path.append("bottom-left-heuristic-master");

	for (int i = 0; i < (int) pieces.size(); i++) {
		Piece* piece = pieces[i];
		destroy_piece(piece);
	}

	
	if (idOS == UNIX) {
		exec_path.append("main");
	} 
	else if (idOS == WINDOWS) {
		exec_path.append("main.exe");
	}
	
	exec_heuristic(
		exec_path.string().c_str(), 
		kiln_file_name, 
		solution_file_name
	);

	convert_tex_solution_to_pdf(solution_file_name, program_solver_dir.string().c_str());

	remove_solver_input_file(solver_input_path.string().c_str());


	delete[] string_endline;
	return 0;
}