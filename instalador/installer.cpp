#include <windows.h>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>

enum {WINDOWS, WINDOWS32, WINDOWS64, LINUX};
int idOS;
int osName;
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
        if (strcmp(install_location_c_string, "\0") == 0) {
            strcat(install_location_c_string, getenv("HOMEDRIVE"));
            strcat(install_location_c_string, getenv("HOMEPATH"));
            fs::path install_dir = install_location_c_string;
            install_dir.append("Documents");
            strcpy(install_location_c_string, install_dir.string().c_str());
        }
        idOS = WINDOWS;
        osName = WINDOWS32;
    #endif

    #ifdef _WIN64
        if (strcmp(install_location_c_string, "\0") == 0) {
            strcat(install_location_c_string, getenv("HOMEDRIVE"));
            strcat(install_location_c_string, getenv("HOMEPATH"));
            fs::path install_dir = install_location_c_string;
            install_dir.append("Documents");
            strcpy(install_location_c_string, install_dir.string().c_str());
        }
        idOS = WINDOWS;
        osName = WINDOWS64;
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


void wchar_message_box_text_to_char(char* converted_text, const wchar_t* text) {
    wcstombs(converted_text, text, 2048);
}

// IDYES == 6; IDNO == 7
int msg_box_YN_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    return MessageBox(0, message, title, MB_ICONQUESTION | MB_YESNO | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}


void msg_box_info_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    MessageBox(0, message, title, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}

void msg_box_error_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    MessageBox(0, message, title, MB_ICONERROR | MB_OK | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}


int remove_by_abs_path_command_line_linux(const char* absolute_path) {
    fs::path file_path = absolute_path;
    if (fs::is_directory(file_path)) {
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
    else {
        char remove_command[512];
        strcpy(remove_command, "rm");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        return exec_command_line(remove_command);
    }
    cout << "Arquivo " << absolute_path << " não encontrado." << endl;
    return 0;
}


int remove_by_abs_path_command_line_windows(const char* absolute_path) {
    fs::path file_path = absolute_path;
    if (fs::is_directory(file_path)) {
        char remove_command[512];
        strcpy(remove_command, "rmdir");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        strcat(remove_command, " ");
        strcat(remove_command, "/s /q");
        return exec_command_line(remove_command, true);
    }
    else {
        char remove_command[512];
        strcpy(remove_command, "del");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        return exec_command_line(remove_command, true);
    }
    cout << "Arquivo " << absolute_path << "não encontrado" << endl;
    return 0;
}

int remove_by_abs_path_command_line(const char* absolute_path) {
    fs::path file_path = absolute_path;
    if (fs::exists(file_path)) {
        if (idOS == LINUX) {
            return remove_by_abs_path_command_line_linux(absolute_path);
        }
        else if (idOS == WINDOWS) {
            return remove_by_abs_path_command_line_windows(absolute_path);
        }
    }
    cout << "Arquivo " << absolute_path << "não encontrado" << endl;
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
    else if (idOS == WINDOWS) {
        char command[256];
        
        strcpy(command, "rmdir /S /Q \"%PROGRAMFILES%\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%PROGRAMFILES(x86)%\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%PROGRAMDATA%\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%APPDATA%\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%LOCALAPPDATA%\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%LOCALAPPDATA%\\Programs\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%ALLUSERSPROFILE%\\Microsoft\\Windows\\Start Menu\\Programs\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "rmdir /S /Q \"%APPDATA%\\Microsoft\\Windows\\Start Menu\\Programs\\MiKTeX\"");
        exec_command_line(command, true);

        strcpy(command, "reg delete HKCU\\Software\\MiKTeX.org /f");
        exec_command_line(command, true);

        strcpy(command, "reg delete HKLM\\SOFTWARE\\MiKTeX.org /f");
        exec_command_line(command, true);

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
        remove_by_abs_path_command_line(zip_file.string().c_str());
    }

    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");

    if (fs::exists(unzip_local)) {
        remove_by_abs_path_command_line(unzip_local.string().c_str());
    }

    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    if (fs::exists(new_project_path) and not project_already_existed) {
         remove_by_abs_path_command_line(new_project_path.string().c_str());
    }
    
    if (not pdflatex_already_installed) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status == 0) {
            remove_pdflatex();
        }
    }
    
    if (idOS == LINUX) {
        if (not unzip_already_installed) {
            int exec_status = exec_command_line("unzip --help");
            if (exec_status == 0) {
                remove_unzip();
            }
        }
    }
    exit(0);
}

void get_download_command_linux(char* command){
    cout << "Baixando arquivos..." << endl;
    strcpy(command, "\0");
    
    strcat(command, "wget");
    strcat(command, " ");

    char download_link[] = "'https://github.com/thuzax/Projeto-Ceramica-Dev/archive/master.zip'";
    strcat(command, download_link);
    strcat(command, " ");

    strcat(command, "--directory-prefix=");
    strcat(command, install_location_c_string);
}

void get_download_command_windows(char* command){
    fs::path dest_dir = install_location_c_string;
    strcpy(command, "curl https://github.com/thuzax/Projeto-Ceramica-Dev/archive/refs/heads/main.zip -L -o ");
    dest_dir.append("main.zip");
    strcat(command, dest_dir.string().c_str());
}


void get_download_command(char* command){
    if (idOS == LINUX) {
        return get_download_command_linux(command);
    }
    else if (idOS == WINDOWS) {
        return get_download_command_windows(command);
    }
    strcpy(command, "\0");
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

void unzip_program_linux() {
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
            // cout << "Abortando operação." << endl;
        //     abort_instalation(false);
        // }

        // cout << "Digite a senha de administrador se necessário:" << endl;

        // exec_status = remove_by_abs_path_command_line_linux(new_project_path.string().c_str());
        // if (exec_status != 0) {
            // cout << "Erro durante a remoção de projeto já existente!" << endl;
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

    exec_status = remove_by_abs_path_command_line(unzip_local.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção de pasta temporária." << endl;
        exit(1);
    }

    exec_status = remove_by_abs_path_command_line(zip_file.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção do arquivo zip baixado." << endl;
        exit(1);
    }
}

void unzip_program_windows() {
    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    
    if (not fs::exists(unzip_local)) {
        char command[512];
        strcpy(command, "mkdir");
        strcat(command, " ");
        strcat(command, unzip_local.string().c_str());
        int exec_status;
        exec_status = exec_command_line(command, true);
        if (exec_status != 0) {
            cout << "Erro na extração do arquivo" << endl;
            wchar_t message[128];
            wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
            wchar_t title[128];
            wcscpy(title, L"Instalação cancelada.");
            msg_box_error_windows(message, title);
            abort_instalation();
        }
    }
    
    
    fs::path zip_file = install_location_c_string;
    zip_file.append("main.zip");

    char command[512];
    strcpy(command, "tar -xf");
    strcat(command, " ");
    strcat(command, zip_file.string().c_str());
    strcat(command, " ");
    strcat(command, "-C");
    strcat(command, " ");
    strcat(command, unzip_local.string().c_str());

    int exec_status;
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        msg_box_error_windows(message, title);
        abort_instalation();
    }


    fs::path project_directory_path;
    for (const auto & file_name : fs::directory_iterator(unzip_local)) {
        project_directory_path = file_name.path();
    }

    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    strcpy(command, "xcopy");
    strcat(command, " ");
    strcat(command, project_directory_path.string().c_str());
    strcat(command, " ");
    strcat(command, new_project_path.string().c_str());
    strcat(command, " ");
    strcat(command, "/s /h /e /k /f /y /i");
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

   
    exec_status = remove_by_abs_path_command_line(unzip_local.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

    exec_status = remove_by_abs_path_command_line(zip_file.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

}


void unzip_program() {
    if (idOS == LINUX) {
        unzip_program_linux();
    }
    else if (idOS == WINDOWS) {
        unzip_program_windows();
    }
}


void download_zip() {
    char command[1024];
    get_download_command(command);
    int execution_status = exec_command_line(command, true);

    if (execution_status != 0) {
        cout << "Erro durante o download do projeto. Abortando instalação."  << endl;
        if (idOS == WINDOWS) {
            wchar_t message[128];
            wcscpy(message, L"Erro durante o download do projeto. Abortando instalação.");
            wchar_t title[128];
            wcscpy(title, L"Instalação cancelada.");
            msg_box_error_windows(message, title);
        }
        abort_instalation();
    }
    cout << command << endl;
}

void login_as_admin_linux() {
    char command[512];
    strcpy(command, "sudo su");
    exec_command_line(command);
}


void install_pdflatex_linux() {
    // cout << endl << endl;
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

    cout << "Instalação do pdflatex concluída." << endl;
}


void install_miktex() {
    char command[128];
    int exec_status;

    cout << "Iniciando instalação do MikTex." << endl;

    fs::path miktexsetup_local_path = install_location_c_string;


    fs::path miktexsetup_zip_path = install_location_c_string;
    miktexsetup_zip_path.append("miktexsetup-x64.zip");
    strcpy(command, "curl https://miktex.org/download/win/miktexsetup-x64.zip -L -o ");
    strcat(command, miktexsetup_zip_path.string().c_str());
    

    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível baixar o miktexsetup-x64.zip. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

    strcpy(command, "tar -xf");
    strcat(command, " ");
    strcat(command, miktexsetup_zip_path.string().c_str());
    strcat(command, " ");
    strcat(command, "-C");
    strcat(command, " ");
    strcat(command, miktexsetup_local_path.string().c_str());
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível descompactar o arquivo. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }


    fs::path miktexsetup_standalone_path = install_location_c_string;
    miktexsetup_standalone_path.append("miktexsetup_standalone.exe");

    strcpy(command, miktexsetup_standalone_path.string().c_str());
    strcat(command, " --quiet --local-package-repository=C:\\temp\\miktex --package-set=basic download");
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível baixar pacotes para instalação. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

    strcpy(command, miktexsetup_standalone_path.string().c_str());
    strcat(command, " --quiet --package-set=basic install");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível finalizar a instalação. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

    strcpy(command, "set PATH=%PATH%;C:\\Program Files\\MiKTeX\\miktex\\bin\\x64");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível alterar as variáveis de caminho. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }




    cout << "Verificando se a instalação foi concluída corretamente." << endl;
    strcpy(command, "pdflatex --version");
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: pdflatex não encontrado. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        msg_box_error_windows(message, title);
        abort_instalation();
    }

    cout << "OK!" << endl;

    remove_by_abs_path_command_line(miktexsetup_zip_path.string().c_str());
    remove_by_abs_path_command_line(miktexsetup_standalone_path.string().c_str());

    // UNINSTALL MIKTEX
    // rmdir /S /Q "%PROGRAMFILES%\MiKTeX"
    // rmdir /S /Q "%PROGRAMFILES(x86)%\MiKTeX"
    // rmdir /S /Q "%PROGRAMDATA%\MiKTeX"
    // rmdir /S /Q "%APPDATA%\MiKTeX"
    // rmdir /S /Q "%LOCALAPPDATA%\MiKTeX"
    // rmdir /S /Q "%LOCALAPPDATA%\Programs\MiKTeX"
    // rmdir /S /Q "%ALLUSERSPROFILE%\Microsoft\Windows\Start Menu\Programs\MiKTeX"
    // rmdir /S /Q "%APPDATA%\Microsoft\Windows\Start Menu\Programs\MiKTeX"
    // reg delete HKCU\Software\MiKTeX.org /f
    // reg delete HKLM\SOFTWARE\MiKTeX.org /f
}


void install_pdflatex() {
    if (idOS == LINUX) {
        cout << "Verificando a instalação do pdflatex..." << endl;
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status != 0) {
            pdflatex_already_installed = false;
            install_pdflatex_linux();
        }
    }
    else if (idOS == WINDOWS) {
        cout << "Verificando a instalação do MikTex/pdflatex..." << endl;
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
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("listas_salvas");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("solucoes_salvas");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("docs");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".git");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".gitignore");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append(".vscode");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

    to_remove = installed_dir.string().c_str();
    to_remove.append("instalador");
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
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
    cout << "Verificando a instalação do LibreOffice 7." << endl;
    cout << "Digite a senha de administrador se necessário." << endl;
    if (idOS == LINUX){
        if (not libreoffice7_is_installed_linux()) {
            cout << "LibreOffice 7 é necessário para a execução deste programa." << endl;
            cout << "Abortando instalação" << endl;
            abort_instalation();
        }
    }
    else if(idOS == WINDOWS) {
        cout << "Verificando a instalação do LibreOffice 7." << endl;
        if (not libreoffice7_is_installed_windows()) {
            cout << "LibreOffice 7 e necessário para a execução deste programa." << endl;
            cout << "Abortando instalacao" << endl;
            wchar_t message[128];
            wcscpy(message, L"LibreOffice 7 e necessário para a execução deste programa.");
            wchar_t title[128];
            wcscpy(title, L"Cancelando instalação");
            msg_box_error_windows(message, title);
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
    cout << "==================================================================" << endl;
    install_pdflatex();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    download_zip();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    unzip_program();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    clean_project();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;

}


void ask_install_permission_windows() {
    wchar_t message[2048];
    wcscpy(message, L"\0");
    wcscat(message, L"É necessário ter instalado o programa");
    wcscat(message, L"  ");
    wcscat(message, L"pdflatex para a geração de pdfs contendo a solução.");
    wcscat(message, L" ");
    wcscat(message, L"Caso ele não esteja em seu computador será necessário instalar o programa MikTex.");
    wcscat(message, L" ");
    wcscat(message, L"Você permite a instalação?");
    
    wchar_t title[128];
    wcscpy(title, L"Permitir instalação");

    int result = msg_box_YN_windows(message, title);
    if (result != IDYES) {
        cout << "Abortando instalação" << endl;
        wchar_t message_error[128];
        wcscpy(message_error, L"A instalação será abortada.");
        wchar_t title_error[128];
        wcscpy(title_error, L"Instalação cancelada.");
        msg_box_error_windows(message_error, title_error);
        abort_instalation();
    }

}

void create_desktop_shortcut_windows() {
    // mklink /H C:\Users\ahscruz\Desktop\teste.ods "C:\Users\ahscruz\Documents\Projeto-Ceramica\forno.ods"
    fs::path file_forno_path = install_location_c_string;
    file_forno_path.append("Projeto-Ceramica");
    file_forno_path.append("forno.ods");
    char user_path_string[128];
    strcpy(user_path_string, getenv("HOMEDRIVE"));
    strcat(user_path_string, getenv("HOMEPATH"));
    fs::path shortcut_path = user_path_string;
    shortcut_path.append("Desktop");
    shortcut_path.append("forno.ods");

    if (fs::exists(shortcut_path)) {
        int answer = msg_box_YN_windows(L"Já existe um arquivo com o nome da aplicação em sua Área de Trabalho. Deseja sobrescrevê-lo?", L"Sobrescrever arquivo de mesmo nome?");
        if (answer == IDYES) {
            remove_by_abs_path_command_line(shortcut_path.string().c_str());
        }
        else {
            answer = msg_box_YN_windows(L"Deseja continuar a instalção sem criar o atalho?", L"Continuar sem criar atalho?");
            if (answer != IDYES) {
                cout << "Abortando instalação." << endl;
                msg_box_error_windows(L"Abortando instalação. Os arquivos e programas intalados serão removidos.", L"Cancelando instalação");
                abort_instalation();
            }
            return;
        }
    }

    char command[256];
    strcpy(command, "mklink");
    strcat(command, " ");
    strcat(command, "/H");
    strcat(command, " ");
    strcat(command, shortcut_path.string().c_str());
    strcat(command, " ");
    strcat(command, file_forno_path.string().c_str());
    
    int exec_status = exec_command_line(command);
    if (exec_status != 0) {
        cout << "Erro na ciração do atalho" << endl;
        int answer = msg_box_YN_windows(L"Ocorreu um erro na criação do atalho para o arquivo forno.ods. Deseja continuar a instação?", L"Erro na instalação");
        if (answer != IDYES) {
            cout << "Abortando instalação." << endl;
            msg_box_error_windows(L"Abortando instalação. Os arquivos e programas intalados serão removidos.", L"Cancelando instalação");
            abort_instalation();
        }
        cout << "Prosseguindo com a instalação" << endl;
    }

}


void install_windows() {
    cout << "==================================================================" << endl;
    ask_install_permission_windows();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    install_pdflatex();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    download_zip();
    cout << "==================================================================" << endl;
    unzip_program();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    create_desktop_shortcut_windows();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
    clean_project();
    cout << "==================================================================" << endl;
    cout << endl << endl << endl;
    cout << "==================================================================" << endl;
}

void verify_admin() {
    if (idOS == LINUX) {
        login_as_admin_linux();
    }
    else if (idOS == WINDOWS) {
        // int result = exec_command_line("mkdir \"C:\\Program Files (x86)\\teste\"", true);
        // if(result != 0) {
        //     cout << "Abortando execução" << endl;
        //     msg_box_error_windows(L"É necessário executar este programa como administrador!", L"Permissão necessária.");
        //     abort_instalation();
        //     exit(0);
        // }
        // else {
        //     remove_by_abs_path_command_line("\"C:\\Program Files (x86)\\teste\"");
        // }
    }
}

void logout_as_admin() {
    if (idOS == LINUX) {
        exec_command_line("exit");
    }
}


void install() {
    verify_admin();
    verify_libreoffice_installed();
    set_project_already_exists();

    if (idOS == LINUX) {
        if (project_already_existed) {
            cout << "Uma instalação deste projeto já existe. Cancelando..." << endl;
            abort_instalation();
        }
        install_linux();
        cout << "Instalação finalizada" << endl;
    }
    else if (idOS == WINDOWS) {
        if (project_already_existed) {
            wchar_t message[128];
            wcscpy(message, L"Uma instalação deste projeto já existe. Cancelando...");
            wchar_t title[128];
            wcscpy(title, L"Cancelando instalação");
            msg_box_error_windows(message, title);
            abort_instalation();
        }
        install_windows();
        wchar_t message[128];
        wcscpy(message, L"Instalação finalizada");
        wchar_t title[128];
        wcscpy(title, L"Fim");
        msg_box_info_windows(message, title);
    }
    
    logout_as_admin();
}

int main() {
    define_OS_data();
    cout << "==================================================================" << endl;
    if (idOS == LINUX) {
        cout << "Iniciando instalação..." << endl;
    }
    else if(idOS == WINDOWS) {
        if (osName == WINDOWS64) {
            SetConsoleOutputCP(CP_UTF8);
        }

        cout << "Iniciando instalação..." << endl;
    }
    else {
        cout << "Sistema Operacional imcompatível." << endl;
        exit(0);
    }
    install();
    return 0;

}