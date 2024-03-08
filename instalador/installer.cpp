// if OS is windows, include windows.h for GUI messages
#if defined(_WIN32) || defined(__WIN64)
    #include <windows.h>
#endif


#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <filesystem>

//  Enumerate OS Names
enum {WINDOWS, WINDOWS32, WINDOWS64, LINUX, UNIX};
// identify which type of OS is being used
int idOS;
// Name the OS that is being used
int osName;
// Instalation directory path as c string
char install_location_c_string[512] = "\0";

// auxliary variables to control already installed elements
bool pdflatex_already_installed = true;
bool unzip_already_installed = true;
bool project_already_existed = true;


char used_version_file[] = "light-version-forno.ods";
char non_used_version_file[] = "forno.ods";

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

using namespace std;

// Define variables that are different in each OS
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

// Execute a system command. Returns 0 if it was a success, otherwise, returns another integer
int exec_command_line(const char* command, bool show_output = false) {
	FILE *fpipe;

	cout << endl << command << endl;

	char line[8192];

    #if defined(_WIN32) || defined(_WIN64)
        fpipe = (FILE*) _popen(command,"r");
    #else
	    fpipe = (FILE*) popen(command,"r");
    #endif
	// If fpipe is NULL
	if (fpipe == nullptr) {  
		perror("Problems with pipe");
		exit(1);
	}

    // Execute and print output
    while (fgets(line, sizeof(line), fpipe)) {
        if (show_output) {
            cout << line << endl;
        }
    }

	return pclose(fpipe);
    
}

// Convert wchar to char (used on windows only)
void wchar_message_box_text_to_char(char* converted_text, const wchar_t* text) {
    wcstombs(converted_text, text, 2048);
}

// Create message boxes for windwos

// Create message with 'yes' and 'no' options
#if defined(_WIN32) || defined(__WIN64)
// IDYES == 6; IDNO == 7
int msg_box_YN_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    return MessageBox(0, message, title, MB_ICONQUESTION | MB_YESNO | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}

// Create message with info icon
void msg_box_info_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    MessageBox(0, message, title, MB_ICONINFORMATION | MB_OK | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}

// Create message with error icon
void msg_box_error_windows(const wchar_t* message_wchar, const wchar_t* title_wchar) {
    char message[2048];
    wchar_message_box_text_to_char(message, message_wchar);
    char title[2048];
    wchar_message_box_text_to_char(title, title_wchar);
    MessageBox(0, message, title, MB_ICONERROR | MB_OK | MB_SETFOREGROUND | MB_SYSTEMMODAL);
}
#endif

// Remove a direcotry or file using the absolute path (linux version)
int remove_by_abs_path_command_line_linux(const char* absolute_path) {
    fs::path file_path = absolute_path;
    // If it is a directory removes with sudo rm -R
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
        // If it is not a directory removes with rm
        char remove_command[512];
        strcpy(remove_command, "rm");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        return exec_command_line(remove_command);
    }
    cout << "Arquivo " << absolute_path << " não encontrado." << endl;
    return 0;
}

// Remove a direcotry or file using the absolute path (windows version)
int remove_by_abs_path_command_line_windows(const char* absolute_path) {
    fs::path file_path = absolute_path;
    if (fs::is_directory(file_path)) {
        // If it is a directory uses rmdir
        char remove_command[512];
        strcpy(remove_command, "rmdir");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        strcat(remove_command, " ");
        strcat(remove_command, "/s /q");
        return exec_command_line(remove_command, true);
    }
    else {
        // If it is a file remove with del
        char remove_command[512];
        strcpy(remove_command, "del");
        strcat(remove_command, " ");
        strcat(remove_command, file_path.string().c_str());
        return exec_command_line(remove_command, true);
    }
    cout << "Arquivo " << absolute_path << " não encontrado" << endl;
    return 0;
}

// Remove using abs path (call the current OS function)
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
    cout << "Arquivo " << absolute_path << " não encontrado" << endl;
    return 0;
}

// Remove pdflatex
void remove_pdflatex() {
    
    if (idOS == LINUX) {
        // Uses apt package manager
        cout << "==================================================================" << endl;
        cout << "Desinstalando pacotes ";
        cout << "texlive-latex-base, texlive-fonts-recommended, texlive-fonts-extra e texlive-latex-extra" << endl;
        cout << "==================================================================" << endl;
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
        // Delete all directories related to miktex
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

// Uninstall unzip (linux only)
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

// Cancel instalation and remove installed packages/programs
void abort_instalation(bool exclude_project = true) {
    cout << "Removendo arquivos criados até o momento..." << endl;

    // Remove git zip file
    fs::path zip_file = install_location_c_string;
    zip_file.append("master.zip");
    if (fs::exists(zip_file)) {
        remove_by_abs_path_command_line(zip_file.string().c_str());
    }

    // Remove temporary directory
    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    if (fs::exists(unzip_local)) {
        remove_by_abs_path_command_line(unzip_local.string().c_str());
    }

    // Remove downloaded project file
    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");
    if (fs::exists(new_project_path) and not project_already_existed) {
         remove_by_abs_path_command_line(new_project_path.string().c_str());
    }
    
    // Uninstall pdflatex if it was not installed
    if (not pdflatex_already_installed) {
        int exec_status = exec_command_line("pdflatex --version");
        if (exec_status == 0) {
            remove_pdflatex();
        }
    }
    
    // Uninstall unzip if it is on linux and it was not installed
    if (idOS == LINUX) {
        if (not unzip_already_installed) {
            int exec_status = exec_command_line("unzip --help");
            if (exec_status == 0) {
                remove_unzip();
            }
        }
    }

    // Delete miktex downloaded files
    if (idOS == WINDOWS) {
        fs::path miktexsetup_zip_path = install_location_c_string;
        miktexsetup_zip_path.append("miktexsetup-x64.zip");
        remove_by_abs_path_command_line(miktexsetup_zip_path.string().c_str());
        fs::path miktexsetup_exe = install_location_c_string;
        miktexsetup_exe.append("miktexsetup_standalone.zip");
        remove_by_abs_path_command_line(miktexsetup_exe.string().c_str());
    }
    exit(0);
}

// Get the command line for downloading a zip on linux
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

// Get the command line for downloading a zip on windows
void get_download_command_windows(char* command){
    fs::path dest_dir = install_location_c_string;
    strcpy(command, "curl https://github.com/thuzax/Projeto-Ceramica-Dev/archive/refs/heads/main.zip -L -o ");
    dest_dir.append("main.zip");
    strcat(command, dest_dir.string().c_str());
}

// Get the command line for downloading a zip according to the OS
void get_download_command(char* command){
    if (idOS == LINUX) {
        return get_download_command_linux(command);
    }
    else if (idOS == WINDOWS) {
        return get_download_command_windows(command);
    }
    strcpy(command, "\0");
}

// Install unzip (linux only)
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
        cout << "Instalação do pacote unzip concluída." << endl;
    }
}

// unzip program (linux only)
void unzip_program_linux() {
    char command[512];
    
    // make paths
    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    fs::path zip_file = install_location_c_string;
    zip_file.append("master.zip");
    
    // run unzip command
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

    // get project directory path
    fs::path project_directory_path;
    for (const auto & file_name : fs::directory_iterator(unzip_local)) {
        project_directory_path = file_name.path();
    }

    // Construct project installation path
    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    // Verify if project already exists
    if (fs::exists(new_project_path)) {
        cout << "================================================================================" << endl;
        cout << "Já existe uma instalação deste projeto. Abortando..." << endl;
        cout << "================================================================================" << endl;
        abort_instalation();
    }

    

    // Move and rename the directory
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

    // Rename the ods file
    fs::path path_to_rename = new_project_path;
    path_to_rename.append(used_version_file);
    fs::path new_file_name = new_project_path;
    new_file_name.append("Fornada.ods");
    strcpy(rename_command, "mv");
    strcat(rename_command, " ");
    strcat(rename_command, path_to_rename.string().c_str());
    strcat(rename_command, " ");
    strcat(rename_command, new_file_name.string().c_str());

    exec_status = exec_command_line(rename_command);
    if (exec_status != 0) {
        cout << "Erro no renomeamento do arquivo." << endl;
        exit(1);

    }

    // Remove temporary directory
    exec_status = remove_by_abs_path_command_line(unzip_local.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção de pasta temporária." << endl;
        exit(1);
    }

    // Remove downloaded zip
    exec_status = remove_by_abs_path_command_line(zip_file.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na remoção do arquivo zip baixado." << endl;
        exit(1);
    }
}

// unzip program (windows only)
void unzip_program_windows() {
    // Create temporary destination path
    fs::path unzip_local = install_location_c_string;
    unzip_local.append("tmp");
    
    if (not fs::exists(unzip_local)) {
        // Create temporary destination if it does not exists
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
            #if defined(_WIN32) || defined(__WIN64)
                msg_box_error_windows(message, title);
            #endif
            abort_instalation();
        }
    }
    
    // Create downloaded file path
    fs::path zip_file = install_location_c_string;
    zip_file.append("main.zip");

    // unzip file with tar
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
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }


    // get path to the downloaded project
    fs::path project_directory_path;
    for (const auto & file_name : fs::directory_iterator(unzip_local)) {
        project_directory_path = file_name.path();
    }

    // Create the new path to the project
    fs::path new_project_path = install_location_c_string;
    new_project_path.append("Projeto-Ceramica");

    // copy the project with the new name
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
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Delete the old directory
    exec_status = remove_by_abs_path_command_line(unzip_local.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Delete the downloaded zip
    exec_status = remove_by_abs_path_command_line(zip_file.string().c_str());
    if (exec_status != 0) {
        cout << "Erro na extração do arquivo" << endl;
        wchar_t message[128];
        wcscpy(message, L"Erro na extração do arquivo. Abortando instalação.");
        wchar_t title[128];
        wcscpy(title, L"Instalação cancelada.");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Rename ods file
    char rename_command[512];
    strcpy(rename_command, "ren");
    strcat(rename_command, " ");
    strcat(rename_command, new_project_path.string().c_str());
    strcat(rename_command, " ");
    strcat(rename_command, used_version_file);
    strcat(rename_command, " ");
    strcat(rename_command, "Fornada.ods");
}

// Call OS unzip function
void unzip_program() {
    if (idOS == LINUX) {
        unzip_program_linux();
    }
    else if (idOS == WINDOWS) {
        unzip_program_windows();
    }
}


// Download the zip containing the program
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
            #if defined(_WIN32) || defined(__WIN64)
                msg_box_error_windows(message, title);
            #endif
        }
        abort_instalation();
    }
}

// Force logint as sudo on linux
void login_as_admin_linux() {
    char command[512];
    // Execute a sudo command to force login
    strcpy(command, "sudo ls");
    exec_command_line(command);
}

// Install the pdflatex package on linux
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

// Install miktex on windows
void install_miktex() {
    char command[256];
    int exec_status;

    cout << "Iniciando instalação do MikTex." << endl;

    // Create miktex path to the zip file
    fs::path miktexsetup_local_path = install_location_c_string;
    fs::path miktexsetup_zip_path = install_location_c_string;
    miktexsetup_zip_path.append("miktexsetup-x64.zip");

    // Download miktex
    strcpy(command, "curl https://miktex.org/download/ctan/systems/win32/miktex/setup/windows-x64/miktexsetup-5.5.0+1763023-x64.zip -L -o ");
    strcat(command, miktexsetup_zip_path.string().c_str());;

    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível baixar o miktexsetup-x64.zip. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // unzip miktex
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
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // download packages
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
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // install packages
    strcpy(command, miktexsetup_standalone_path.string().c_str());
    strcat(command, " --quiet --package-set=basic install");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível finalizar a instalação. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Add miktex to PATH
    strcpy(command, "set PATH=%PATH%;C:\\Program Files\\MiKTeX\\miktex\\bin\\x64");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível alterar as variáveis de caminho. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Update miktex packages to complete version
    strcpy(command, "mpm --verbose --package-level=complete --update");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível alterar as variáveis de caminho. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Upgrade miktex to complete version
    strcpy(command, "mpm --verbose --package-level=complete --upgrade");
    exec_status = exec_command_line(command, true);

    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: não foi possível alterar as variáveis de caminho. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    // Verify if pdflatex is installed
    cout << "Verificando se a instalação foi concluída corretamente." << endl;
    strcpy(command, "pdflatex --version");
    exec_status = exec_command_line(command, true);
    if (exec_status != 0) {
        wchar_t message[128];
        wcscpy(message, L"Erro ao instalar o MikTex: pdflatex não encontrado. Abortando operações...");
        wchar_t title[128];
        wcscpy(title, L"Cancelando Instalação");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_error_windows(message, title);
        #endif
        abort_instalation();
    }

    cout << "OK!" << endl;

    // Remove miktex zip and installer
    remove_by_abs_path_command_line(miktexsetup_zip_path.string().c_str());
    remove_by_abs_path_command_line(miktexsetup_standalone_path.string().c_str());

    // COMMANDS TO UNINSTALL MIKTEX
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

// Install the pdflatex command line for the current OS
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

// Remove auxiliary directories present on github
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

    to_remove = installed_dir.string().c_str();
    to_remove.append(non_used_version_file);
    exec_status = remove_by_abs_path_command_line(to_remove.string().c_str());
    if (exec_status != 0) {
        cout << "Erro ao tentar remover " << to_remove.string().c_str();
    }

}

// execute a given system command to verify if libreoffice is installed
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

// run command test to verify wheter libreoffice is on linux or not
bool libreoffice7_is_installed_linux() {
    char command[] = "sudo libreoffice --version";
    return run_test_libreoffice_installed(command);

}

// run command test to verify wheter libreoffice is on windows or not
bool libreoffice7_is_installed_windows() {
    char command[] = "wmic product where \"Name like '%LibreOffice 7%'\" get Name";
    return run_test_libreoffice_installed(command);

}

// check if libreoffice is installed
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
            #if defined(_WIN32) || defined(__WIN64)
                msg_box_error_windows(message, title);
            #endif
            abort_instalation();
        }
    }
}

// verify if the project directory is already installed
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

// install project on linux
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

// Ask permission to install project on windows
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

    #if defined(_WIN32) || defined(__WIN64)
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
    #endif

}

// Create shortcut for the installed sheet on windwos
void create_desktop_shortcut_windows() {
    // mklink C:\Users\ahscruz\Desktop\Fornada.ods "C:\Users\ahscruz\Documents\Projeto-Ceramica\Fornada.ods"
    
    // Construct the path to the project main file
    fs::path file_forno_path = install_location_c_string;
    file_forno_path.append("Projeto-Ceramica");
    file_forno_path.append("Fornada.ods");
    
    char user_path_string[128];
    strcpy(user_path_string, getenv("HOMEDRIVE"));
    strcat(user_path_string, getenv("HOMEPATH"));
    
    // Construct path to the shortcut on desktop
    fs::path shortcut_path = user_path_string;
    shortcut_path.append("Desktop");
    shortcut_path.append("Fornada.ods");

    // verify if a shortcut with same name already exists
    if (fs::exists(shortcut_path)) {
        #if defined(_WIN32) || defined(__WIN64)
            
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

        #endif
    }

    // Run the command to create the shortcut
    char command[256];
    strcpy(command, "mklink");
    strcat(command, " ");
    strcat(command, "");
    strcat(command, " ");
    strcat(command, shortcut_path.string().c_str());
    strcat(command, " ");
    strcat(command, file_forno_path.string().c_str());
    
    int exec_status = exec_command_line(command);
    if (exec_status != 0) {
        cout << "Erro na ciração do atalho" << endl;
        #if defined(_WIN32) || defined(__WIN64)
            int answer = msg_box_YN_windows(L"Ocorreu um erro na criação do atalho para o arquivo Fornada.ods. Deseja continuar a instação?", L"Erro na instalação");
            if (answer != IDYES) {
                cout << "Abortando instalação." << endl;
                msg_box_error_windows(L"Abortando instalação. Os arquivos e programas intalados serão removidos.", L"Cancelando instalação");
                abort_instalation();
            }
            cout << "Prosseguindo com a instalação" << endl;
        #endif
    }

}


// install project on windows
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

// verify if user is admin, if linux, log in
void verify_admin() {
    if (idOS == LINUX) {
        login_as_admin_linux();
    }
    else if (idOS == WINDOWS) {
        char command[256];
        strcpy(command, "net session");
        if (exec_command_line(command) != 0) {
            wchar_t message_error[128];
            wcscpy(message_error, L"É necessária a execução como administrador. Abortando instalação.\n");
            wcscat(message_error, L"Os arquivos e programas intalados serão removidos.");
            msg_box_error_windows(message_error, L"Permissão Negada!");
            abort_instalation();
        }
    }
}

// exit sudo su
void logout_as_admin() {
    if (idOS == LINUX) {
        exec_command_line("exit");
    }
}


// install the project according to the OS
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
            #if defined(_WIN32) || defined(__WIN64)
                msg_box_error_windows(message, title);
            #endif
            abort_instalation();
        }
        install_windows();
        wchar_t message[128];
        wcscpy(message, L"Instalação finalizada");
        wchar_t title[128];
        wcscpy(title, L"Fim");
        #if defined(_WIN32) || defined(__WIN64)
            msg_box_info_windows(message, title);
        #endif
    }
    
    logout_as_admin();
}

int main() {
    // start global OS variables
    define_OS_data();
    cout << "==================================================================" << endl;
    if (idOS == LINUX) {
        cout << "Iniciando instalação..." << endl;
    }
    else if(idOS == WINDOWS) {
        // set the console to use utf-8
        #if defined(__WIN64)
            SetConsoleOutputCP(CP_UTF8);
        #endif

        cout << "Iniciando instalação..." << endl;
    }
    else {
        // if not windows nor linux, cancel
        cout << "Sistema Operacional imcompatível." << endl;
        exit(0);
    }
    install();
    return 0;

}
