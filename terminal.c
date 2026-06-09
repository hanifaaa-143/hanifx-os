#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>

#define VERSION "5.0.0"
#define MAX_CMD 256
#define HISTORY_SIZE 50
#define CONFIG_PATH "/data/data/com.termux/files/home/.hanifx/config"
#define LOG_PATH "/data/data/com.termux/files/home/.hanifx/login.log"
#define MOTD_PATH "/data/data/com.termux/files/home/.hanifx/motd"

char history[HISTORY_SIZE][MAX_CMD];
int history_count = 0;
char current_user[64] = "Hanif";

void add_history(char *cmd) {
    if (history_count < HISTORY_SIZE) {
        strncpy(history[history_count++], cmd, MAX_CMD);
    }
}

void hide_input(char *buf, int size) {
    struct termios old, new_t;
    tcgetattr(STDIN_FILENO, &old);
    new_t = old;
    new_t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_t);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    printf("\n");
}

void load_config() {
    FILE *cfg = fopen(CONFIG_PATH, "r");
    if (cfg) {
        char line[128];
        while (fgets(line, sizeof(line), cfg)) {
            if (strncmp(line, "username=", 9) == 0) {
                strncpy(current_user, line + 9, sizeof(current_user));
                current_user[strcspn(current_user, "\n")] = 0;
            }
        }
        fclose(cfg);
    }
}

int hanifx_login() {
    char pass[64];
    char stored_pass[64] = "hanifx123";
    FILE *cfg = fopen(CONFIG_PATH, "r");
    if (cfg) {
        char line[128];
        while (fgets(line, sizeof(line), cfg)) {
            if (strncmp(line, "password=", 9) == 0) {
                strncpy(stored_pass, line + 9, sizeof(stored_pass));
                stored_pass[strcspn(stored_pass, "\n")] = 0;
            }
        }
        fclose(cfg);
    }
    int attempts = 3;
    printf("\033[1;36m");
    printf("  +================================+\n");
    printf("  |       HanifX OS v5.0.0         |\n");
    printf("  |      Secure Login System       |\n");
    printf("  +================================+\n");
    printf("\033[0m\n");
    while (attempts > 0) {
        printf("\033[1;33m[LOGIN]\033[0m Password: ");
        fflush(stdout);
        hide_input(pass, sizeof(pass));
        if (strcmp(pass, stored_pass) == 0) {
            printf("\033[1;32m[OK] Welcome back, %s!\033[0m\n\n", current_user);
            FILE *log = fopen(LOG_PATH, "a");
            if (log) {
                time_t t = time(NULL);
                fprintf(log, "[LOGIN] %s: %s", current_user, ctime(&t));
                fclose(log);
            }
            return 1;
        } else {
            attempts--;
            if (attempts > 0)
                printf("\033[1;31m[ERR] Wrong! %d attempts left\033[0m\n", attempts);
        }
    }
    FILE *log = fopen(LOG_PATH, "a");
    if (log) {
        time_t t = time(NULL);
        fprintf(log, "[FAILED] %s", ctime(&t));
        fclose(log);
    }
    printf("\033[1;31m[LOCKED] Too many attempts. Shutting down...\033[0m\n");
    return 0;
}

void hanifx_banner() {
    printf("\033[1;36m");
    printf("  _   _             _  ___ __  __  ___  ____\n");
    printf(" | | | | __ _ _ __ (_)/ __|  \\/  |/ _ \\/ ___|\n");
    printf(" | |_| |/ _` | '_ \\| |\\ \\| |\\/| | | | \\___ \\\n");
    printf(" |  _  | (_| | | | | | > >| |  | | |_| |___) |\n");
    printf(" |_| |_|\\__,_|_| |_|_|/___|_|  |_|\\___/|____/\n");
    printf("\033[1;32m");
    printf("        HanifX OS Terminal v%s\n", VERSION);
    printf("        Mobile-First OS by %s\n", current_user);
    printf("\033[0m\n");
}

void hanifx_motd() {
    FILE *f = fopen(MOTD_PATH, "r");
    if (f) {
        printf("\033[1;33m[MOTD]\033[0m\n");
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            printf("  %s", line);
        }
        fclose(f);
        printf("\n");
    }
}

void hanifx_help() {
    printf("\033[1;33m[HanifX Commands]\033[0m\n");
    printf("  help       - Show all commands\n");
    printf("  about      - About HanifX OS\n");
    printf("  clear      - Clear screen\n");
    printf("  sysinfo    - Show system info\n");
    printf("  files      - List files\n");
    printf("  newfile    - Create new file\n");
    printf("  read       - Read a file\n");
    printf("  delete     - Delete a file\n");
    printf("  calc       - Calculator\n");
    printf("  ping       - Ping a host\n");
    printf("  whoami     - Current user info\n");
    printf("  datetime   - Show date and time\n");
    printf("  monitor    - Live system monitor\n");
    printf("  mkdir      - Create directory\n");
    printf("  cd         - Change directory\n");
    printf("  history    - Command history\n");
    printf("  encrypt    - Encrypt a file\n");
    printf("  decrypt    - Decrypt a file\n");
    printf("  netinfo    - Network information\n");
    printf("  pkg        - HanifX package manager\n");
    printf("  script     - Run a script file\n");
    printf("  portscan   - Scan ports of a host\n");
    printf("  netmon     - Network monitor\n");
    printf("  ipinfo     - IP information\n");
    printf("  myip       - Show my IP address\n");
    printf("  loginlog   - Show login history\n");
    printf("  passwd     - Change password\n");
    printf("  setmotd    - Set welcome message\n");
    printf("  reboot     - Restart HanifX OS\n");
    printf("  exit       - Exit terminal\n");
}

void hanifx_about() {
    printf("\033[1;35m");
    printf("  +================================+\n");
    printf("  |       HanifX OS v5.0.0         |\n");
    printf("  |   Mobile-First Custom OS       |\n");
    printf("  |   Built by: %-19s|\n", current_user);
    printf("  |   Platform: Android/Termux     |\n");
    printf("  |   Security: Password Protected |\n");
    printf("  |   Phase: 6 FINAL               |\n");
    printf("  +================================+\n");
    printf("\033[0m");
}

void hanifx_sysinfo() {
    printf("\033[1;32m[System Info]\033[0m\n");
    printf("  OS      : HanifX OS v5.0.0\n");
    printf("  User    : %s\n", current_user);
    system("echo '  Kernel  :' $(uname -r)");
    system("echo '  Arch    :' $(uname -m)");
    system("echo '  Uptime  :' $(uptime -p)");
    system("free -h | awk '/Mem/{print \"  Memory  : \" $3\"/\"$2}'");
    system("echo '  CPU     :' $(nproc) 'cores'");
    system("df -h $HOME | awk 'NR==2{print \"  Storage : \" $3\"/\"$2}'");
}

void hanifx_files() {
    printf("\033[1;34m[Files]\033[0m\n");
    system("ls -la --color=auto");
}

void hanifx_newfile(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: newfile <filename>\n");
        return;
    }
    FILE *f = fopen(filename, "w");
    if (f) {
        printf("Type content (blank line to save):\n");
        char line[256];
        while (fgets(line, sizeof(line), stdin)) {
            if (strcmp(line, "\n") == 0) break;
            fputs(line, f);
        }
        fclose(f);
        printf("\033[1;32m[OK] File '%s' saved!\033[0m\n", filename);
    } else {
        printf("\033[1;31m[ERR] Could not create file!\033[0m\n");
    }
}

void hanifx_read(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: read <filename>\n");
        return;
    }
    FILE *f = fopen(filename, "r");
    if (f) {
        printf("\033[1;34m[%s]\033[0m\n", filename);
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            printf("%s", line);
        }
        fclose(f);
    } else {
        printf("\033[1;31m[ERR] File not found!\033[0m\n");
    }
}

void hanifx_delete(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: delete <filename>\n");
        return;
    }
    if (remove(filename) == 0) {
        printf("\033[1;32m[OK] File '%s' deleted!\033[0m\n", filename);
    } else {
        printf("\033[1;31m[ERR] Could not delete file!\033[0m\n");
    }
}

void hanifx_calc(char *expr) {
    if (strlen(expr) == 0) {
        printf("Usage: calc <expression>\n");
        return;
    }
    double a, b, result;
    char op;
    if (sscanf(expr, "%lf %c %lf", &a, &op, &b) == 3) {
        switch(op) {
            case '+': result = a + b; break;
            case '-': result = a - b; break;
            case '*': result = a * b; break;
            case '/':
                if (b == 0) {
                    printf("\033[1;31m[ERR] Cannot divide by zero!\033[0m\n");
                    return;
                }
                result = a / b; break;
            default:
                printf("\033[1;31m[ERR] Unknown operator!\033[0m\n");
                return;
        }
        printf("\033[1;32m[CALC] %.2lf %c %.2lf = %.2lf\033[0m\n", a, op, b, result);
    } else {
        printf("\033[1;31m[ERR] Invalid expression!\033[0m\n");
    }
}

void hanifx_ping(char *host) {
    if (strlen(host) == 0) {
        printf("Usage: ping <host>\n");
        return;
    }
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "ping -c 4 %s", host);
    printf("\033[1;34m[PING] %s\033[0m\n", host);
    system(cmd);
}

void hanifx_whoami() {
    printf("\033[1;33m[User Info]\033[0m\n");
    printf("  User    : %s\n", current_user);
    system("echo '  Home    :' $HOME");
    printf("  Shell   : HanifX Terminal v5.0.0\n");
    system("echo '  Host    :' $(hostname)");
    system("echo '  Device  :' $(uname -m)");
}

void hanifx_datetime() {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("\033[1;32m[DateTime] %s\033[0m\n", buf);
}

void hanifx_monitor() {
    printf("\033[1;33m[Live Monitor] Press Ctrl+C to stop\033[0m\n");
    system("watch -n 1 'free -h | awk \"/Mem/{print \\\"Memory: \\\" \\$3\\\"/\\\"\\$2}\"; uptime'");
}

void hanifx_mkdir(char *dirname) {
    if (strlen(dirname) == 0) {
        printf("Usage: mkdir <dirname>\n");
        return;
    }
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "mkdir -p %s", dirname);
    if (system(cmd) == 0) {
        printf("\033[1;32m[OK] Directory '%s' created!\033[0m\n", dirname);
    } else {
        printf("\033[1;31m[ERR] Could not create directory!\033[0m\n");
    }
}

void hanifx_history() {
    printf("\033[1;33m[Command History]\033[0m\n");
    if (history_count == 0) {
        printf("  No history yet.\n");
        return;
    }
    for (int i = 0; i < history_count; i++) {
        printf("  %d  %s\n", i + 1, history[i]);
    }
}

void hanifx_encrypt(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: encrypt <filename>\n");
        return;
    }
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("\033[1;31m[ERR] File not found!\033[0m\n");
        return;
    }
    char outname[256];
    snprintf(outname, sizeof(outname), "%s.hxenc", filename);
    FILE *out = fopen(outname, "wb");
    if (!out) {
        fclose(f);
        printf("\033[1;31m[ERR] Cannot create encrypted file!\033[0m\n");
        return;
    }
    int c;
    int key = 0x4E;
    while ((c = fgetc(f)) != EOF) {
        fputc(c ^ key, out);
    }
    fclose(f);
    fclose(out);
    remove(filename);
    printf("\033[1;32m[OK] Encrypted: %s -> %s\033[0m\n", filename, outname);
}

void hanifx_decrypt(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: decrypt <filename>\n");
        return;
    }
    FILE *f = fopen(filename, "rb");
    if (!f) {
        printf("\033[1;31m[ERR] File not found!\033[0m\n");
        return;
    }
    char outname[256];
    strncpy(outname, filename, sizeof(outname));
    char *ext = strstr(outname, ".hxenc");
    if (ext) *ext = 0;
    else snprintf(outname, sizeof(outname), "%s.dec", filename);
    FILE *out = fopen(outname, "wb");
    if (!out) {
        fclose(f);
        printf("\033[1;31m[ERR] Cannot create decrypted file!\033[0m\n");
        return;
    }
    int c;
    int key = 0x4E;
    while ((c = fgetc(f)) != EOF) {
        fputc(c ^ key, out);
    }
    fclose(f);
    fclose(out);
    remove(filename);
    printf("\033[1;32m[OK] Decrypted: %s -> %s\033[0m\n", filename, outname);
}

void hanifx_netinfo() {
    printf("\033[1;34m[Network Info]\033[0m\n");
    system("ip addr show 2>/dev/null | grep 'inet ' | awk '{print \"  IP      : \" $2}'");
    system("cat /etc/resolv.conf 2>/dev/null | grep nameserver | awk '{print \"  DNS     : \" $2}'");
    system("ip route 2>/dev/null | grep default | awk '{print \"  Gateway : \" $3}'");
}

void hanifx_pkg(char *arg) {
    if (strlen(arg) == 0) {
        printf("\033[1;33m[HanifX Package Manager]\033[0m\n");
        printf("  pkg install <name>\n");
        printf("  pkg remove <name>\n");
        printf("  pkg list\n");
        return;
    }
    char action[64], pkgname[128];
    memset(action, 0, sizeof(action));
    memset(pkgname, 0, sizeof(pkgname));
    sscanf(arg, "%s %s", action, pkgname);
    char cmd[256];
    if (strcmp(action, "install") == 0 && strlen(pkgname) > 0) {
        snprintf(cmd, sizeof(cmd), "pkg install %s -y", pkgname);
        printf("\033[1;34m[PKG] Installing %s...\033[0m\n", pkgname);
        system(cmd);
    } else if (strcmp(action, "remove") == 0 && strlen(pkgname) > 0) {
        snprintf(cmd, sizeof(cmd), "pkg remove %s -y", pkgname);
        printf("\033[1;34m[PKG] Removing %s...\033[0m\n", pkgname);
        system(cmd);
    } else if (strcmp(action, "list") == 0) {
        printf("\033[1;34m[PKG] Installed packages:\033[0m\n");
        system("pkg list-installed 2>/dev/null");
    } else {
        printf("\033[1;31m[ERR] Unknown pkg command!\033[0m\n");
    }
}

void hanifx_script(char *filename) {
    if (strlen(filename) == 0) {
        printf("Usage: script <filename>\n");
        return;
    }
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("\033[1;31m[ERR] Script not found!\033[0m\n");
        return;
    }
    printf("\033[1;34m[SCRIPT] Running: %s\033[0m\n", filename);
    char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0 || line[0] == '#') continue;
        printf("\033[1;33m> %s\033[0m\n", line);
        system(line);
    }
    fclose(f);
    printf("\033[1;32m[OK] Script done!\033[0m\n");
}

void hanifx_portscan(char *host) {
    if (strlen(host) == 0) {
        printf("Usage: portscan <host/ip>\n");
        return;
    }
    char cmd[512];
    printf("\033[1;34m[PORTSCAN] Scanning %s...\033[0m\n", host);
    snprintf(cmd, sizeof(cmd), "nmap -T4 --top-ports 100 %s", host);
    system(cmd);
}

void hanifx_netmon() {
    printf("\033[1;33m[NETMON] Network Monitor - Ctrl+C to stop\033[0m\n");
    system("watch -n 2 'cat /proc/net/dev | awk \"NR>2{print \\$1, \\\"RX:\\\", \\$2, \\\"TX:\\\", \\$10}\"'");
}

void hanifx_ipinfo(char *ip) {
    if (strlen(ip) == 0) {
        printf("Usage: ipinfo <ip>\n");
        return;
    }
    char cmd[512];
    printf("\033[1;34m[IPINFO] %s\033[0m\n", ip);
    snprintf(cmd, sizeof(cmd), "curl -s ipinfo.io/%s", ip);
    system(cmd);
    printf("\n");
}

void hanifx_myip() {
    printf("\033[1;33m[MY IP]\033[0m\n");
    system("echo '  Public IP :' $(curl -s ifconfig.me)");
    system("ip addr show 2>/dev/null | grep 'inet ' | awk '{print \"  Local IP  : \" $2}'");
}

void hanifx_loginlog() {
    printf("\033[1;33m[Login History]\033[0m\n");
    FILE *f = fopen(LOG_PATH, "r");
    if (f) {
        char line[256];
        while (fgets(line, sizeof(line), f)) {
            if (strstr(line, "[LOGIN]"))
                printf("\033[1;32m  %s\033[0m", line);
            else if (strstr(line, "[FAILED]"))
                printf("\033[1;31m  %s\033[0m", line);
        }
        fclose(f);
    } else {
        printf("  No log found.\n");
    }
}

void hanifx_passwd() {
    char old[64], new1[64], new2[64];
    char stored[64] = "hanifx123";
    FILE *cfg = fopen(CONFIG_PATH, "r");
    if (cfg) {
        char line[128];
        while (fgets(line, sizeof(line), cfg)) {
            if (strncmp(line, "password=", 9) == 0) {
                strncpy(stored, line + 9, sizeof(stored));
                stored[strcspn(stored, "\n")] = 0;
            }
        }
        fclose(cfg);
    }
    printf("Current password: ");
    hide_input(old, sizeof(old));
    if (strcmp(old, stored) != 0) {
        printf("\033[1;31m[ERR] Wrong password!\033[0m\n");
        return;
    }
    printf("New password: ");
    hide_input(new1, sizeof(new1));
    printf("Confirm password: ");
    hide_input(new2, sizeof(new2));
    if (strcmp(new1, new2) != 0) {
        printf("\033[1;31m[ERR] Passwords do not match!\033[0m\n");
        return;
    }
    char username_line[128] = "";
    cfg = fopen(CONFIG_PATH, "r");
    if (cfg) {
        char line[128];
        while (fgets(line, sizeof(line), cfg)) {
            if (strncmp(line, "username=", 9) == 0) {
                strncpy(username_line, line, sizeof(username_line));
            }
        }
        fclose(cfg);
    }
    FILE *out = fopen(CONFIG_PATH, "w");
    if (out) {
        if (strlen(username_line) > 0) fputs(username_line, out);
        fprintf(out, "password=%s\n", new1);
        fclose(out);
        printf("\033[1;32m[OK] Password changed!\033[0m\n");
    }
}

void hanifx_setmotd(char *msg) {
    if (strlen(msg) == 0) {
        printf("Usage: setmotd <message>\n");
        return;
    }
    FILE *f = fopen(MOTD_PATH, "w");
    if (f) {
        fprintf(f, "%s\n", msg);
        fclose(f);
        printf("\033[1;32m[OK] MOTD set!\033[0m\n");
    }
}

void hanifx_reboot() {
    printf("\033[1;33m[HanifX OS] Rebooting...\033[0m\n");
    sleep(1);
    execv("/proc/self/exe", (char *[]){"/proc/self/exe", NULL});
}

int main() {
    load_config();
    if (!hanifx_login()) return 1;

    char input[MAX_CMD];
    char cmd[MAX_CMD];
    char arg[MAX_CMD];

    hanifx_banner();
    hanifx_motd();
    hanifx_help();

    while (1) {
        printf("\033[1;36m%s@hanifx\033[0m:\033[1;33m~\033[0m$ ", current_user);
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (strlen(input) == 0) continue;

        memset(cmd, 0, sizeof(cmd));
        memset(arg, 0, sizeof(arg));
        sscanf(input, "%s %[^\n]", cmd, arg);
        add_history(input);

        if (strcmp(cmd, "exit") == 0) {
            printf("\033[1;31m[HanifX OS] Shutting down...\033[0m\n");
            break;
        } else if (strcmp(cmd, "help") == 0) {
            hanifx_help();
        } else if (strcmp(cmd, "about") == 0) {
            hanifx_about();
        } else if (strcmp(cmd, "clear") == 0) {
            system("clear");
            hanifx_banner();
        } else if (strcmp(cmd, "sysinfo") == 0) {
            hanifx_sysinfo();
        } else if (strcmp(cmd, "files") == 0) {
            hanifx_files();
        } else if (strcmp(cmd, "newfile") == 0) {
            hanifx_newfile(arg);
        } else if (strcmp(cmd, "read") == 0) {
            hanifx_read(arg);
        } else if (strcmp(cmd, "delete") == 0) {
            hanifx_delete(arg);
        } else if (strcmp(cmd, "calc") == 0) {
            hanifx_calc(arg);
        } else if (strcmp(cmd, "ping") == 0) {
            hanifx_ping(arg);
        } else if (strcmp(cmd, "whoami") == 0) {
            hanifx_whoami();
        } else if (strcmp(cmd, "datetime") == 0) {
            hanifx_datetime();
        } else if (strcmp(cmd, "monitor") == 0) {
            hanifx_monitor();
        } else if (strcmp(cmd, "mkdir") == 0) {
            hanifx_mkdir(arg);
        } else if (strcmp(cmd, "history") == 0) {
            hanifx_history();
        } else if (strcmp(cmd, "encrypt") == 0) {
            hanifx_encrypt(arg);
        } else if (strcmp(cmd, "decrypt") == 0) {
            hanifx_decrypt(arg);
        } else if (strcmp(cmd, "netinfo") == 0) {
            hanifx_netinfo();
        } else if (strcmp(cmd, "pkg") == 0) {
            hanifx_pkg(arg);
        } else if (strcmp(cmd, "script") == 0) {
            hanifx_script(arg);
        } else if (strcmp(cmd, "portscan") == 0) {
            hanifx_portscan(arg);
        } else if (strcmp(cmd, "netmon") == 0) {
            hanifx_netmon();
        } else if (strcmp(cmd, "ipinfo") == 0) {
            hanifx_ipinfo(arg);
        } else if (strcmp(cmd, "myip") == 0) {
            hanifx_myip();
        } else if (strcmp(cmd, "loginlog") == 0) {
            hanifx_loginlog();
        } else if (strcmp(cmd, "passwd") == 0) {
            hanifx_passwd();
        } else if (strcmp(cmd, "setmotd") == 0) {
            hanifx_setmotd(arg);
        } else if (strcmp(cmd, "reboot") == 0) {
            hanifx_reboot();
        } else if (strcmp(cmd, "cd") == 0) {
            if (strlen(arg) == 0) {
                chdir(getenv("HOME"));
            } else {
                if (chdir(arg) != 0)
                    printf("\033[1;31m[ERR] Directory not found!\033[0m\n");
            }
        } else if (strlen(cmd) > 0) {
            printf("\033[1;31m[ERR] '%s' not found. Type 'help'\033[0m\n", cmd);
        }
    }
    return 0;
}
