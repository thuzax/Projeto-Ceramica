#include <iostream>
#include <math.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <filesystem>

#include<stdio.h>
// sudo apt install texlive-latex-base texlive-fonts-recommended texlive-fonts-extra texlive-latex-extra
// miktexsetup --package-set=basic download
// miktexsetup install

using namespace std;


enum {WINDOWS, UNIX};

#ifdef __WIN32 || _WIN64
	int idOS = WINDOWS;
#endif

#ifdef __unix__
	int idOS = UNIX;
#endif



enum {SQUARE, RECTANGLE, TRIANGLE, CIRCLE};

const char* lable_circle = "Círculo";
const char* lable_square = "Quadrado";
const char* lable_rectangle = "Retângulo";
const char* lable_triangle = "Triângulo";

const char* solver_input_name = "solver_input.txt";

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

	char line[256];

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
	
	char command[256] = "\0";
	strcat(command, "pdflatex");
	strcat(command, " ");
	strcat(command, "--interaction=batchmode");
	// strcat(command, "--interaction=nonstopmode");
	strcat(command, " ");
	strcat(command, "-output-directory=");
	strcat(command, program_root_dir);
	strcat(command, " ");
	strcat(command, solution_file_name);

	exec_command_line(command);

	strcpy(command, "\0");
	
	filesystem::path dir = program_root_dir;

	dir.append("*.aux");
	char aux_files[256];
	strcpy(aux_files, dir.string().c_str());
	// strcat(aux_files, "*.aux");
	
	dir = dir.parent_path();
	
	dir.append("*.log");	
	char log_files[256];
	strcpy(log_files, dir.string().c_str());
	// strcat(log_files, "*.log");
	
	dir = dir.parent_path();
	
	dir.append("*.synctex.gz");
	char synctexgz_files[256];
	strcpy(synctexgz_files, dir.string().c_str());
	// strcat(synctexgz_files, "*.synctex.gz");
	
	dir = dir.parent_path();
	
	dir.append("*.tex");	
	char tex_files[256];
	strcpy(tex_files, dir.string().c_str());
	// strcat(tex_files, "*.tex");

	dir = dir.parent_path();

	strcat(command, "rm -f");
	strcat(command, " ");
	strcat(command, aux_files);
	strcat(command, " ");
	strcat(command, log_files);
	strcat(command, " ");
	strcat(command, synctexgz_files);
	strcat(command, " ");
	strcat(command, tex_files);

	exec_command_line(command);
}


void exec_heuristic(const char* solver_path, const char* kiln_file_name, const char* solution_file_name) {
	char command[256] = "\0";
	strcat(command, solver_path);
	strcat(command, " ");
	strcat(command, kiln_file_name);
	strcat(command, " ");
	strcat(command, solver_input_name);
	strcat(command, " ");
	strcat(command, solution_file_name);

	exec_command_line(command);
}

void remove_solver_input_file(const char* solver_input_name) {
	char command[256] = "\0";
	strcat(command, "rm -f");
	strcat(command, " ");
	strcat(command, solver_input_name);

	exec_command_line(command);

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

	ofstream file_instance;
	file_instance.open(solver_input_name);

	file_instance << number_of_pieces << "\n";
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
			
			file_instance << piece->description << "\n";
		}
	}

	file_instance.close();

	for (int i = 0; i < (int) pieces.size(); i++) {
		Piece* piece = pieces[i];
		destroy_piece(piece);
	}

	filesystem::path p = argv[0];
	filesystem::path exec_path = p.parent_path();
	exec_path.append("bottom-left-heuristic-master");
	filesystem::path program_solver_dir = exec_path.parent_path();
	
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

	remove_solver_input_file(solver_input_name);

	return 0;
}