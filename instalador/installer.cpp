#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>

enum {WINDOWS, LINUX};
int idOS;
char install_location_c_string[512] = "\0";


bool pdflatex_already_installed = true;
bool unzip_already_installed = true;
bool project_already_existed = true;

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

using namespace std;

void define_OS_data() {
    #ifdef __WIN32
        // strcat(install_location_c_string, getenv("HOMEDRIVE"));
        strcat(install_location_c_string, getenv("HOMEPATH"));
        idOS = WINDOWS;
    #endif

    #ifdef _WIN64
        // strcat(install_location_c_string, getenv("HOMEDRIVE"));
        strcat(install_location_c_string, getenv("HOMEPATH"));
        idOS = WINDOWS;
    #endif

    #ifdef __linux__
        strcat(install_location_c_string, getenv("HOME"));
        // strcat(install_location_c_string, "/Documentos");
        idOS = LINUX;
    #endif
}


int exec_command_line(const char* command, bool show_output = false) {
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
        if (show_output) {
            cout << line << endl;
        }
    }

	
	return pclose(fpipe);
    
}


void DisplayConfirmSaveAsMessageBox() {
}

// IDYES == 6; IDNO == 7
int msg_box_YN_windows(const char* message, const char* title = "Confirme") {
    return MessageBox(0, message, title, MB_YESNO);
}


void msg_box_info_windows(const char* message, const char* title = "Informacao") {
    MessageBox(0, message, title, MB_OK );
}


int remove_by_abs_path_command_line_linux(const char* absolute_path) {
    fs::path file_path = absolute_path;
    if (fs::exists(file_path)) {
        if (fs::is_directory(file_path)) {
            if (idOS == LINUX) {
                cout << "Removendo diretório " << absolute_path << endl;
                cout << "Digite a senha de administrador se necessário." << endl;
                char remove_command[512];
                strcpy(remove_command, "sudo");
                strcat(remove_command, " ");
                strcat(remove_command, "rm");
                strcat(remove_command, " ");
                strcat(remove_command, "-R");
                strcat(remove_command, " ");
                strcat(remove_command, file_path.string().c_str());
                return exec_command_line(remove_command);
            }
        }
        else {
            if (idOS == LINUX) {
                char remove_command[512];
                strcpy(remove_command, "rm");
                strcat(remove_command, " ");
                strcat(remove_command, file_path.string().c_str());
                return exec_command_line(remove_command);
            }
        }
    }
    cout << "Arquivo " << absolute_path << " não encontrado." << endl;
    return 0;
}

void remove_pdflatex() {
    cout << "==================================================================" << endl;
    cout << "Desinstalando pacotes ";
    cout << "texlive-latex-base, texlive-fonts-recommended, texlive-fonts-extra e texlive-latex-extra" << endl;
    cout << "==================================================================" << endl;
    if (idOS == LINUX) {
        char command[256];
        strcpy(command, "sudo apt autoremove -y");
        strcat(command, " ");
        strcat(command, "texlive-latex-base");
        strcat(command, " ");
        strcat(command, "texlive-fonts-recommended");
        strcat(command, " ");
        strcat(command, "texlive-fonts-extra");
        strcat(command, " ");
        strcat(command, "texlive-latex-extra");
        exec_command_line(command);
    }
}

void remove_unzip() {
    cout << "==================================================================" << endl;
    cout << "Desinstalando pacote unzip" << endl;
    cout << "==================================================================" << endl;
    if (idOS == LINUX) {
        char command[256];
        strcpy(command, "sudo apt autoremove -y");
        strcat(command, " ");
        strcat(command, "unzip");
        exec_command_line(command);
    }
}


void abort_instalation(bool exclude_project = true) {
    fs::path zip_file = install_location_c_string;
    zip_file.append("master.zip");

    cout << "Removendo arquivos criados até o momento..." << endl;

    if (fs::exists(zip_file)) {
        remove_by_abs_path_command_line_linux(zip_file.string().c_str());
    }

    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");

    if (fs::exists(unzip_local)) {
        remove_by_abs_path_command_line_linux(unzip_local.string().c_str());
    }

    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    if (fs::exists(new_project_path) and not project_already_existed) {
         remove_by_abs_path_command_line_linux(new_project_path.string().c_str());
    }
    
    if (not pdflatex_already_installed) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status == 0) {
            remove_pdflatex();
        }
    }

    if (not unzip_already_installed) {
        int exec_status = exec_command_line("unzip --help");
        if (exec_status == 0) {
            remove_unzip();
        }
    }

    exit(0);

}

char* get_download_command_linux(char* command){
    cout << "Baixando arquivos..." << endl;
    strcpy(command, "\0");
    
    strcat(command, "wget");
    strcat(command, " ");

    char download_link[] = "'https://github.com/thuzax/Projeto-Ceramica-Dev/archive/master.zip'";
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
        cout << "==================================================================" << endl;
        cout << "Instalando o pacote unzip." << endl;
        cout << "(Digite a senha de administrador se necessário.)" << endl;
        cout << "==================================================================" << endl;
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
    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    fs::path zip_file = install_location_c_string;
    zip_file.append("master.zip");
    
    strcpy(command, "unzip -o -d");
    strcat(command, " ");
    strcat(command, unzip_local.string().c_str());
    strcat(command, " ");
    strcat(command, zip_file.string().c_str());

    int exec_status = exec_command_line(command);
    if (exec_status != 0) {
        cout << "Erro na extração de arquivos!" << endl;
        abort_instalation();
        exit(1);
    }

    fs::path project_directory_path;
    for (const auto & file_name : fs::directory_iterator(unzip_local)) {
        project_directory_path = file_name.path();
    }
    
    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    if (fs::exists(new_project_path)) {
        cout << "================================================================================" << endl;
        cout << "Já existe uma instalação deste projeto. Abortando..." << endl;
        cout << "================================================================================" << endl;
        abort_instalation();
        // cout << "Já existe uma cópia do do projeto instalada. ";
        // cout << "Continuar a instalação irá remover a instalação antiga e todos os arquivos relacionados. ";
        // cout << "Deseja continuar? [S/n]  ";
        // string answer;
        // getline(cin, answer);

        // if (answer == "n" or answer == "N") {
        //     cout << "Abortando operação." << endl;
        //     abort_instalation(false);
        // }

        // cout << "Digite a senha de administrador se necessário:" << endl;

        // exec_status = remove_by_abs_path_command_line_linux(new_project_path.string().c_str());
        // if (exec_status != 0) {
        //     cout << "Erro durante a remoção de projeto já existente!" << endl;
        //     abort_instalation();
        // }

    }

    char rename_command[512];
    strcpy(rename_command, "mv");
    strcat(rename_command, " ");
    strcat(rename_command, project_directory_path.string().c_str());
    strcat(rename_command, " ");
    strcat(rename_command, new_project_path.string().c_str());

    exec_status = exec_command_line(rename_command);
    if (exec_status != 0) {
        cout << "Erro no renomeamento do arquivo." << endl;
        exit(1);

    }

    exec_status = remove_by_abs_path_command_line_linux(unzip_local.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção de pasta temporária." << endl;
        exit(1);
    }

    exec_status = remove_by_abs_path_command_line_linux(zip_file.string().c_str());
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
    cout << endl << endl;
    cout << "==================================================================" << endl;
    cout << "Instalando pacotes texlive-latex-base, texlive-fonts-recommended, texlive-fonts-extra e texlive-latex-extra. Aguarde..." << endl;
    cout << "(Digite a senha de administrador se necessário)" << endl;
    cout << "==================================================================" << endl;
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
}


void install_miktex() {
    char command[128];
    int exec_status;

    strcpy(command, "curl -LO https://miktex.org/download/win/miktexsetup-x64.zip");
    exec_status = exec_command_line(command);
    if (exec_status != 0) {
        msg_box_info_windows("Erro ao instalar o miktex. Abortando operacoes...", "Erro de instalacao");
        abort_instalation();
    }

    strcpy(command, "miktexsetup_standalone --verbose --local-package-repository=C:\\temp\\miktex --package-set=basic download");
    exec_status = exec_command_line(command);
    if (exec_status != 0) {
        msg_box_info_windows("Erro ao instalar o miktex. Abortando operacoes...", "Erro de instalacao");
        abort_instalation();
    }

    strcpy(command, "miktexsetup_standalone --verbose --package-set=basic install");
    exec_status = exec_command_line(command);

    if (exec_status != 0) {
        msg_box_info_windows("Erro ao instalar o miktex. Abortando operacoes...", "Erro de instalacao");
        abort_instalation();
    }

    strcpy(command, "set PATH=%PATH%;C:\\Program Files\\MiKTeX\\miktex\\bin\\x64");
    exec_status = exec_command_line(command);

    if (exec_status != 0) {
        msg_box_info_windows("Erro ao instalar o miktex. Abortando operacoes...", "Erro de instalacao");
        abort_instalation();
    }



    // UNINSTALL MIKTEX
    // rmdir /S /Q "%PROGRAMFILES%\MiKTeX 2.9"
    // rmdir /S /Q "%PROGRAMFILES(x86)%\MiKTeX 2.9"
    // rmdir /S /Q "%PROGRAMDATA%\MiKTeX"
    // rmdir /S /Q "%APPDATA%\MiKTeX"
    // rmdir /S /Q "%LOCALAPPDATA%\MiKTeX"
    // rmdir /S /Q "%LOCALAPPDATA%\Programs\MiKTeX 2.9"
    // rmdir /S /Q "%ALLUSERSPROFILE%\Microsoft\Windows\Start Menu\Programs\MiKTeX 2.9"
    // rmdir /S /Q "%APPDATA%\Microsoft\Windows\Start Menu\Programs\MiKTeX 2.9"
    // reg delete HKCU\Software\MiKTeX.org
    // reg delete HKLM\SOFTWARE\MiKTeX.org
}


void install_pdflatex() {
    if (idOS == LINUX) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status != 0) {
            pdflatex_already_installed = false;
            install_pdflatex_linux();
        }
    }
    else if (idOS == WINDOWS) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status != 0) {
            pdflatex_already_installed = false;
            install_miktex();
        }
    }
}


void clean_project() {
    fs::path installed_dir = install_location_c_string;

    installed_dir.append("Projeto-Ceramica");
    int exec_status;
    fs::path to_remove = installed_dir.string().c_str();


    to_remove.append("images");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("listas_salvas");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("solucoes_salvas");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("docs");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".git");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".gitignore");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".vscode");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("instalador");
    exec_status = remove_by_abs_path_command_line_linux(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }
}


bool run_test_libreoffice_installed(const char* command) {
    FILE *fpipe;
	char line[8192];
	fpipe = (FILE*) popen(command,"r");
	// If fpipe is NULL
	if (fpipe == nullptr) {  
		perror("Problems with pipe");
		exit(1);
	}
    int end = -1;
    bool isInstalled = false;
    while (fgets(line, sizeof(line), fpipe)) {
        if (isInstalled) {
            continue;
        }
        char teste[14] = "\0";
        strncpy(teste, line, 13);

        cout << line << endl;
        
        if (strcmp(teste, "LibreOffice 7") == 0) {
            isInstalled = true;
        }
    }
    return isInstalled;
}


bool libreoffice7_is_installed_linux() {
    char command[] = "sudo libreoffice --version";
    return run_test_libreoffice_installed(command);

}


bool libreoffice7_is_installed_windows() {
    char command[] = "wmic product where \"Name like '%LibreOffice 7%'\" get Name";
    return run_test_libreoffice_installed(command);

}


void verify_libreoffice_installed() {
    if (idOS == LINUX){
        cout << "Verificando a instalação do LibreOffice 7." << endl;
        cout << "Digite a senha de administrador se necessário." << endl;
        if (not libreoffice7_is_installed_linux()) {
            cout << "LibreOffice 7 é necessário para a execução deste programa." << endl;
            cout << "Abortando instalação" << endl;
            abort_instalation();
        }
    }
    else if(idOS == WINDOWS) {
        cout << "Verificando a instalacao do LibreOffice 7." << endl;
        if (not libreoffice7_is_installed_windows()) {
            cout << "LibreOffice 7 e necessario para a execucao deste programa." << endl;
            cout << "Abortando instalacao" << endl;
            abort_instalation();
        }
    }
}

void set_project_already_exists() {
    fs::path installed_dir = install_location_c_string;
    installed_dir.append("Projeto-Ceramica");

    if (fs::exists(installed_dir)) {
        project_already_existed = true;
    }
    else {
        project_already_existed = false;
    }
}

void install_linux() {
    set_project_already_exists();

    cout << "==================================================================" << endl;
    cout << "Caso não estejam instalados, será necessário os pacotes: " << endl;
    cout << "  - unzip para descompactar o arquivo com o código fonte;" << endl;
    cout << "  - texlive-latex-base, texlive-fonts-recommended, texlive-fonts-extra e texlive-latex-extra ";
    cout << "para geração de arquivos pdf a partir de documentos latex;" << endl;
    cout << "Você permite a instalação? [S/n] ";
    string answer;
    getline(cin, answer);
    if (answer == "n" or answer == "N") {
        cout << "Abortando operação." << endl;
        abort_instalation(false);
    }
    cout << "==================================================================" << endl;
    
    cout << endl << endl << endl;
    int exec_status = exec_command_line("unzip --help");
    if (exec_status != 0) {
        unzip_already_installed = false;
        install_unzip();
    }
    install_pdflatex();
    cout << endl << endl << endl;
    download_zip();
    cout << endl << endl << endl;
    unzip_program();
    cout << endl << endl << endl;
    clean_project();
    cout << endl << endl << endl;
}

void ask_install_permission_windows() {
    char message[128];
    strcpy(message, "\0");
    strcat(message, "E necessario instalar os seguintes programas:");
    strcat(message, "  ");
    strcat(message, "miktex.");
    strcat(message, " ");
    strcat(message, "Voce permite a instalacao?");
    
    int result = msg_box_YN_windows(message);

    if (result != IDYES) {
        cout << "Abortando instalacao" << endl;
        msg_box_info_windows("A instalacao sera abortada");
        abort_instalation();
    }
}


void install_windows() {
    set_project_already_exists();
    ask_install_permission_windows();
    install_pdflatex();
}

void install() {
    verify_libreoffice_installed();
    if (idOS == LINUX) {
        install_linux();
        cour << "Instalação finalizada" << endl;
    }
    else if (idOS == WINDOWS) {
        install_windows();
        msg_box_info_windows("Instalacao concluida", "Fim");
    }
}

int main() {
    define_OS_data();
    cout << "==================================================================" << endl;
    if (idOS == LINUX) {
        cout << "Iniciando instalação..." << endl;
    }
    else if(idOS == WINDOWS) {
        cout << "Iniciando instalacao..." << endl;
    }
    else {
        cout << "Sistema Operacional imcompatível." << endl;
        exit(0);
    }
    install();
    return 0;

}