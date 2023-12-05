#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>


enum {WINDOWS, LINUX};
int idOS;
char install_location_c_string[512] = "\0";

bool installed_pdflatex = false;

using namespace std;

void define_OS_data() {
    #ifdef __WIN32
        strcat(install_location_c_string, getenv("HOMEDRIVE"));
        strcat(install_location_c_string, getenv("HOMEPATH"));
        idOS = WINDOWS;
    #endif

    #ifdef _WIN64
        strcat(install_location_c_string, getenv("HOMEDRIVE"));
        strcat(install_location_c_string, getenv("HOMEPATH"));
        idOS = WINDOWS;
    #endif

    #ifdef __linux__
        strcat(install_location_c_string, getenv("HOME"));
        strcat(install_location_c_string, "/Documentos");
        idOS = LINUX;
    #endif
}


int exec_command_line(const char* command) {
	FILE *fpipe;

	cout << endl << command << endl;

	char line[8192];

	fpipe = (FILE*) popen(command,"r");
	// If fpipe is NULL
	if (fpipe == nullptr) {  
		perror("Problems with pipe");
		exit(1);
	}

	while (fgets(line, sizeof(line), fpipe)) {
		cout << line << endl;
	}
	
	return pclose(fpipe);
    
}


int remove_by_abs_path_command_line(const char* absolute_path) {
    filesystem::path file_path = absolute_path;
    if (filesystem::exists(file_path)) {
        if (filesystem::is_directory(file_path)) {
            cout << "Removendo diretório " << absolute_path << endl;
            cout << "Digite a senha de administrador se necessário." << endl;
            char remove_command[512];
            strcpy(remove_command, "sudo");
            strcat(remove_command, " ");
            strcat(remove_command, "rm");
            strcat(remove_command, " ");
            strcat(remove_command, "-R");
            strcat(remove_command, " ");
            strcat(remove_command, file_path.c_str());
            return exec_command_line(remove_command);
        }
        else {
            char remove_command[512];
            strcpy(remove_command, "rm");
            strcat(remove_command, " ");
            strcat(remove_command, file_path.c_str());
            return exec_command_line(remove_command);

        }

    }
    cout << "Arquivo " << absolute_path << " não encontrado." << endl;
    return 0;
}

void abort_instalation(bool exclude_project = true) {
    filesystem::path zip_file = install_location_c_string;
    zip_file.append("master.zip");

    cout << "Removendo arquivos criados até o momento..." << endl;

    if (filesystem::exists(zip_file)) {
        remove_by_abs_path_command_line(zip_file.c_str());
    }

    filesystem::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");

    if (filesystem::exists(unzip_local)) {
        remove_by_abs_path_command_line(unzip_local.c_str());
    }

    filesystem::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    if (filesystem::exists(new_project_path) and exclude_project) {
        remove_by_abs_path_command_line(new_project_path.c_str());
    }

    exit(0);

}

char* get_download_command_linux(char* command){
    cout << "Baixando arquivos..." << endl;
    strcpy(command, "\0");
    
    strcat(command, "wget");
    strcat(command, " ");

    char download_link[] = "'https://github.com/thuzax/Projeto-Ceramica/archive/master.zip'";
    strcat(command, download_link);
    strcat(command, " ");

    strcat(command, "--directory-prefix=");
    strcat(command, install_location_c_string);

    return command;
}

char* get_download_command_windows(char* command){
    // todo
    strcpy(command, "\0");
    return command;
}


char* get_download_command(char* command){
    if (idOS == LINUX) {
        return get_download_command_linux(command);
    }
    else if (idOS == WINDOWS) {
        return get_download_command_windows(command);
    }
    strcpy(command, "\0");
    return command;
}

void install_unzip() {
    if (idOS == LINUX) {
        cout << "Instalando o pacote unzip. Digite a senha de administrador se necessário." << endl;
        char command[512] = "sudo apt -y install unzip";
        int exec_status = exec_command_line(command);
        if (exec_status != 0) {
            cout << "Erro! Abortando instalação." << endl;
            abort_instalation();
        }
    }
}

void unzip_program() {
    char command[512];
    filesystem::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    filesystem::path zip_file = install_location_c_string;
    zip_file.append("master.zip");
    
    strcpy(command, "unzip -o -d");
    strcat(command, " ");
    strcat(command, unzip_local.c_str());
    strcat(command, " ");
    strcat(command, zip_file.c_str());

    int exec_status = exec_command_line(command);
    if (exec_status != 0) {
        cout << "Erro na extração de arquivos!" << endl;
        abort_instalation();
        exit(1);
    }

    filesystem::path project_directory_path;
    for (const auto & file_name : filesystem::directory_iterator(unzip_local)) {
        project_directory_path = file_name.path();
    }
    
    filesystem::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    if (filesystem::exists(new_project_path)) {
        cout << "Já existe uma cópido do projeto instalada. ";
        cout << "Continuar a instalação irá remover a instalação antiga e todos os arquivos relacionados. ";
        cout << "Deseja continuar? [S/n]  ";
        string answer;
        getline(cin, answer);

        if (answer == "n" or answer == "N") {
            cout << "Abortando operação." << endl;
            abort_instalation(false);
        }

        cout << "Digite a senha de administrador se necessário:" << endl;

        exec_status = remove_by_abs_path_command_line(new_project_path.c_str());
        if (exec_status != 0) {
            cout << "Erro durante a remoção de projeto já existente!" << endl;
            abort_instalation();
        }

    }

    char rename_command[512];
    strcpy(rename_command, "mv");
    strcat(rename_command, " ");
    strcat(rename_command, project_directory_path.c_str());
    strcat(rename_command, " ");
    strcat(rename_command, new_project_path.c_str());

    exec_status = exec_command_line(rename_command);
    if (exec_status != 0) {
        cout << "Erro no renomeamento do arquivo." << endl;
        exit(1);

    }

    exec_status = remove_by_abs_path_command_line(unzip_local.c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção de pasta temporária." << endl;
        exit(1);
    }

    exec_status = remove_by_abs_path_command_line(zip_file.c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção do arquivo zip baixado." << endl;
        exit(1);
    }
}


void download_zip() {
    char command[1024];
    get_download_command(command);
    int execution_status = exec_command_line(command);

    if (execution_status != 0) {
        cout << "Erro durante o download do projeto. Abortando instalação."  << endl;
        abort_instalation();
    }
}

void login_as_admin_linux() {
    char command[512];
    strcpy(command, "sudo su");
    // exec_command_line(command);
}


void install_pdflatex_linux() {
    cout << "Instalando o pdflatex. Digite a senha de administrador se necessário." << endl;
    char command[512];
    strcpy(command, "sudo apt install -y");
    strcat(command, " ");
    strcat(command, "texlive-latex-base");
    strcat(command, " ");
    strcat(command, "texlive-fonts-recommended");
    strcat(command, " ");
    strcat(command, "texlive-fonts-extra");
    strcat(command, " ");
    strcat(command, "texlive-latex-extra");
    
    int exec_status = exec_command_line(command);
    if (exec_status != 0) {
        cout << "Erro na instalação do pdflatex." << endl;
        abort_instalation();
    }
    installed_pdflatex = true;
}


void install_pdflatex() {
    if (idOS == LINUX) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status != 0) {
            install_pdflatex_linux();
        }
    }
    else if (idOS == WINDOWS) {

    }
}

void clean_project() {
    filesystem::path installed_dir = install_location_c_string;

    installed_dir.append("Projeto-Ceramica");
    int exec_status;
    filesystem::path to_remove = installed_dir.c_str();


    to_remove.append("images");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append("listas_salvas");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append("solucoes_salvas");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append("docs");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append(".git");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append(".gitignore");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append(".vscode");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }

    to_remove = installed_dir.c_str();
    to_remove.append("instalador");
    exec_status = remove_by_abs_path_command_line(to_remove.c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.c_str();
    }
}

void install() {
    download_zip();
    if (idOS == LINUX) {
        int exec_status = exec_command_line("unzip --help");
        if (exec_status != 0) {
            install_unzip();
        }
    }
    cout << endl << endl << endl;
    unzip_program();
    cout << endl << endl << endl;
    install_pdflatex();
    clean_project();
}

int main() {
    define_OS_data();
    cout << "Iniciando instalação..." << endl;
    install();
    return 0;

}
