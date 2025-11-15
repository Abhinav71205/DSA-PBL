#define _WIN32_IE 0x0500
#define _WIN32_WINNT 0x0500
#define WINVER 0x0500
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef USER_FILE
#define USER_FILE "users.txt"
#endif
#ifndef ADMIN_FILE
#define ADMIN_FILE "admins.txt"
#endif
#include "user_auth.h"
#include "admin_auth.h"
#include "inventory.h"
#include "cart_order.h"
#include "billing.h"
#include "order_history.h"
#define ID_BTN_USER     1001
#define ID_BTN_ADMIN    1002
#define ID_EDT_USER     1101
#define ID_EDT_PASS     1102
#define ID_BTN_LOGIN    1103
#define ID_BTN_SIGNUP   1104
#define ID_BTN_BACK     1105
#define ID_U_BROWSE     1201
#define ID_U_ADD        1202
#define ID_U_VIEWCART   1203
#define ID_U_CHECKOUT   1204
#define ID_U_HISTORY    1205
#define ID_U_LOGOUT     1299
#define ID_BACK_BROWSE  1400
#define ID_BACK_CART    1401
#define ID_BACK_HISTORY 1402
#define ID_BACK_ADD     1403
#define ID_ADD_CONFIRM  1404
#define ID_ADMIN_LIST   2001
#define ID_ADMIN_ADD    2002
#define ID_ADMIN_UPDATE 2003
#define ID_ADMIN_REMOVE 2004
#define ID_ADMIN_SAVE   2005
#define ID_ADMIN_LOGOUT 2006
#define ID_ADMIN_BACK   2007
#define ID_ADMIN_ADD_CONFIRM    2101
#define ID_ADMIN_UPDATE_CONFIRM 2102
#define ID_ADMIN_REMOVE_CONFIRM 2103
static int g_isAdmin = 0;
static char g_username[64] = "";

Inventory inv;
Cart cart;
LRESULT CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);

void ShowMainMenu(HWND);
void ShowLogin(HWND, int);
void ShowUserDash(HWND);
void ShowAdminDash(HWND);
void ShowProductList(HWND);
void ShowAddToCartWindow(HWND);
void ShowCartWindow(HWND);
void ShowHistoryWindow(HWND);
void ShowAdminListProducts(HWND);
void ShowAdminAddProduct(HWND);
void ShowAdminUpdateQty(HWND);
void ShowAdminRemoveProduct(HWND);
void ClearChildren(HWND);
static void trim_crlf(char *s) {
    if (!s) return;
    size_t n = strlen(s);
    while (n>0 && (s[n-1]=='\n' || s[n-1]=='\r' || s[n-1]==' ' || s[n-1]=='\t')) { s[n-1]=0; n--; }
    while (*s && (*s==' ' || *s=='\t')) { memmove(s,s+1,strlen(s)); }
}
static int username_exists_in_file(const char *file, const char *username) {
    FILE *f = fopen(file, "r");
    if (!f) return 0;
    char ru[128], rp[128];
    int found = 0;
    while (fscanf(f, "%127s %127s", ru, rp) == 2) {
        trim_crlf(ru);
        if (strcmp(ru, username) == 0) { found = 1; break; }
    }
    fclose(f);
    return found;
}
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int ncmdshow) {
    INITCOMMONCONTROLSEX icex = { sizeof(INITCOMMONCONTROLSEX), ICC_LISTVIEW_CLASSES };
    InitCommonControlsEx(&icex);
    WNDCLASSA wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = "EComSystem";
    wc.lpfnWndProc = MainProc;
    if(!RegisterClassA(&wc)) return -1;
    init_inventory(&inv);
    load_inventory_from_file(&inv, "products.txt");
    init_cart(&cart);
    CreateWindowA("EComSystem", "E-Commerce System",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                  520, 240, 520, 420, NULL, NULL, hInst, NULL);
    MSG msg;
    while(GetMessage(&msg,NULL,0,0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    save_inventory_to_file(&inv, "products.txt");
    clear_cart(&cart);
    free_inventory(&inv);
    return 0;
}
void ClearChildren(HWND hwnd) {
    HWND child = GetWindow(hwnd, GW_CHILD);
    while(child) {
        HWND next = GetWindow(child, GW_HWNDNEXT);
        DestroyWindow(child);
        child = next;
    }
}
void ShowMainMenu(HWND hwnd) {
    ClearChildren(hwnd);
    CreateWindowA("static", "Welcome to E-Commerce System",
                  WS_VISIBLE|WS_CHILD|SS_CENTER,
                  120, 40, 260, 28, hwnd, NULL, NULL, NULL);
    CreateWindowA("button", "Login as User",
                  WS_VISIBLE|WS_CHILD,
                  160, 100, 200, 40, hwnd, (HMENU)ID_BTN_USER, NULL, NULL);
    CreateWindowA("button", "Login as Admin",
                  WS_VISIBLE|WS_CHILD,
                  160, 160, 200, 40, hwnd, (HMENU)ID_BTN_ADMIN, NULL, NULL);
}
void ShowLogin(HWND hwnd, int isAdmin) {
    ClearChildren(hwnd);
    g_isAdmin = isAdmin;
    const char *title = isAdmin ? "Admin Login / Sign Up" : "User Login / Sign Up";
    CreateWindowA("static", title, WS_VISIBLE|WS_CHILD|SS_CENTER,
                  140, 30, 230, 24, hwnd, NULL, NULL, NULL);
    CreateWindowA("static", "Username:", WS_VISIBLE|WS_CHILD,
                  100, 80, 90, 24, hwnd, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  200, 80, 200, 24, hwnd, (HMENU)ID_EDT_USER, NULL, NULL);
    CreateWindowA("static", "Password:", WS_VISIBLE|WS_CHILD,
                  100, 120, 90, 24, hwnd, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER|ES_PASSWORD,
                  200, 120, 200, 24, hwnd, (HMENU)ID_EDT_PASS, NULL, NULL);
    CreateWindowA("button", "Login",
                  WS_VISIBLE|WS_CHILD,
                  140, 180, 100, 32, hwnd, (HMENU)ID_BTN_LOGIN, NULL, NULL);
    CreateWindowA("button", "Sign Up",
                  WS_VISIBLE|WS_CHILD,
                  260, 180, 100, 32, hwnd, (HMENU)ID_BTN_SIGNUP, NULL, NULL);
    CreateWindowA("button", "Back",
                  WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 26, hwnd, (HMENU)ID_BTN_BACK, NULL, NULL);
}
void ShowUserDash(HWND hwnd) {
    ClearChildren(hwnd);
    char header[128];
    wsprintfA(header, "User Dashboard - Welcome, %s", g_username);
    CreateWindowA("static", header, WS_VISIBLE|WS_CHILD|SS_CENTER,
                  100, 30, 300, 24, hwnd, NULL, NULL, NULL);
    CreateWindowA("button", "Browse Products", WS_VISIBLE|WS_CHILD,
                  160, 80, 200, 36, hwnd, (HMENU)ID_U_BROWSE, NULL, NULL);
    CreateWindowA("button", "Add to Cart", WS_VISIBLE|WS_CHILD,
                  160, 126, 200, 36, hwnd, (HMENU)ID_U_ADD, NULL, NULL);
    CreateWindowA("button", "View Cart", WS_VISIBLE|WS_CHILD,
                  160, 172, 200, 36, hwnd, (HMENU)ID_U_VIEWCART, NULL, NULL);
    CreateWindowA("button", "Checkout", WS_VISIBLE|WS_CHILD,
                  160, 218, 200, 36, hwnd, (HMENU)ID_U_CHECKOUT, NULL, NULL);
    CreateWindowA("button", "View Order History", WS_VISIBLE|WS_CHILD,
                  160, 264, 200, 36, hwnd, (HMENU)ID_U_HISTORY, NULL, NULL);
    CreateWindowA("button", "Logout", WS_VISIBLE|WS_CHILD,
                  160, 310, 200, 36, hwnd, (HMENU)ID_U_LOGOUT, NULL, NULL);
}
void ShowAdminDash(HWND hwnd) {
    ClearChildren(hwnd);
    char header[128];
    wsprintfA(header, "Admin Dashboard - Welcome, %s", g_username);
    CreateWindowA("static", header, WS_VISIBLE|WS_CHILD|SS_CENTER,
                  100, 30, 300, 24, hwnd, NULL, NULL, NULL);
    CreateWindowA("button", "List products", WS_VISIBLE|WS_CHILD,
                  160, 80, 200, 36, hwnd, (HMENU)ID_ADMIN_LIST, NULL, NULL);
    CreateWindowA("button", "Add product", WS_VISIBLE|WS_CHILD,
                  160, 126, 200, 36, hwnd, (HMENU)ID_ADMIN_ADD, NULL, NULL);
    CreateWindowA("button", "Update qty", WS_VISIBLE|WS_CHILD,
                  160, 172, 200, 36, hwnd, (HMENU)ID_ADMIN_UPDATE, NULL, NULL);
    CreateWindowA("button", "Remove product", WS_VISIBLE|WS_CHILD,
                  160, 218, 200, 36, hwnd, (HMENU)ID_ADMIN_REMOVE, NULL, NULL);
    CreateWindowA("button", "Save & Exit", WS_VISIBLE|WS_CHILD,
                  160, 264, 200, 36, hwnd, (HMENU)ID_ADMIN_SAVE, NULL, NULL);
}
void ShowProductList(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 26, hwndParent, (HMENU)ID_BACK_BROWSE, NULL, NULL);
    HWND list = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
        WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SINGLESEL,
        30, 60, 440, 250, hwndParent, NULL, NULL, NULL);
    LVCOLUMNA col = {0};
    col.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    col.pszText = "ID"; col.cx = 60; ListView_InsertColumn(list, 0, &col);
    col.pszText = "Name"; col.cx = 150; ListView_InsertColumn(list, 1, &col);
    col.pszText = "Price"; col.cx = 100; ListView_InsertColumn(list, 2, &col);
    col.pszText = "Qty"; col.cx = 80; ListView_InsertColumn(list, 3, &col);
    int index = 0;
    Product *cur = inv.head;
    char buf[64];
    while (cur) {
        LVITEMA item = {0};
        item.mask = LVIF_TEXT;
        item.iItem = index;
        sprintf(buf, "%d", cur->id);
        item.pszText = buf;
        ListView_InsertItem(list, &item);
        ListView_SetItemText(list, index, 1, cur->name);
        sprintf(buf, "%.2f", cur->price);
        ListView_SetItemText(list, index, 2, buf);
        sprintf(buf, "%d", cur->qty);
        ListView_SetItemText(list, index, 3, buf);
        cur = cur->next;
        index++;
    }
}
void ShowAddToCartWindow(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 26, hwndParent, (HMENU)ID_BACK_ADD, NULL, NULL);
    CreateWindowA("static", "Product ID:", WS_VISIBLE|WS_CHILD,
                  120, 120, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  240, 120, 160, 24, hwndParent, (HMENU)201, NULL, NULL);
    CreateWindowA("static", "Quantity:", WS_VISIBLE|WS_CHILD,
                  120, 160, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  240, 160, 160, 24, hwndParent, (HMENU)202, NULL, NULL);
    CreateWindowA("button", "Add to Cart", WS_VISIBLE|WS_CHILD,
                  190, 210, 140, 32, hwndParent, (HMENU)ID_ADD_CONFIRM, NULL, NULL);
}
void ShowCartWindow(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 26, hwndParent, (HMENU)ID_BACK_CART, NULL, NULL);
    HWND list = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
        WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SINGLESEL,
        30, 60, 440, 250, hwndParent, NULL, NULL, NULL);
    LVCOLUMNA col = {0};
    col.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
    col.pszText = "Product"; col.cx = 150; ListView_InsertColumn(list, 0, &col);
    col.pszText = "Qty"; col.cx = 80; ListView_InsertColumn(list, 1, &col);
    col.pszText = "Total Price"; col.cx = 100; ListView_InsertColumn(list, 2, &col);
    int index = 0;
    CartItem *cur = cart.head;
    char buf[64];
    while (cur) {
        Product *p = find_product_by_id(&inv, cur->product_id);
        if (p) {
            LVITEMA item = {0};
            item.mask = LVIF_TEXT;
            item.iItem = index;
            item.pszText = p->name;
            ListView_InsertItem(list, &item);
            sprintf(buf, "%d", cur->qty);
            ListView_SetItemText(list, index, 1, buf);
            sprintf(buf, "%.2f", p->price * cur->qty);
            ListView_SetItemText(list, index, 2, buf);
            index++;
        }
        cur = cur->next;
    }
}
void ShowHistoryWindow(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 26, hwndParent, (HMENU)ID_BACK_HISTORY, NULL, NULL);
    HWND list = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
        WS_VISIBLE|WS_CHILD|LVS_REPORT|WS_VSCROLL,
        30, 60, 440, 250, hwndParent, NULL, NULL, NULL);

    LVCOLUMNA col = {0};
    col.mask = LVCF_TEXT|LVCF_WIDTH;
    col.pszText = "Order History"; col.cx = 420; ListView_InsertColumn(list, 0, &col);
    FILE *f = fopen("orders.txt", "r");
    if (!f) {
        MessageBoxA(hwndParent, "No order history yet.", "Info", MB_OK);
        return;
    }
    char line[300];
    int show = 0, index = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strstr(line, "User: ") && strstr(line, g_username)) {
            show = 1;
            LVITEMA header = {0};
            header.mask = LVIF_TEXT;
            header.iItem = index++;
            header.pszText = "--------------------------------";
            ListView_InsertItem(list, &header);
        }
        if (show) {
            LVITEMA item = {0};
            item.mask = LVIF_TEXT;
            item.iItem = index++;
            item.pszText = line;
            ListView_InsertItem(list, &item);

            if (strstr(line, "Total:")) {
                LVITEMA footer = {0};
                footer.mask = LVIF_TEXT;
                footer.iItem = index++;
                footer.pszText = "--------------------------------";
                ListView_InsertItem(list, &footer);
                show = 0;
            }
        }
    }
    fclose(f);
    if (index == 0) MessageBoxA(hwndParent, "No previous orders found.", "Info", MB_OK);
}
void ShowAdminListProducts(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 28, hwndParent, (HMENU)ID_ADMIN_BACK, NULL, NULL);
    HWND list = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
        WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SINGLESEL,
        30, 60, 440, 260, hwndParent, NULL, NULL, NULL);
    LVCOLUMNA col = {0};
    col.mask = LVCF_TEXT|LVCF_WIDTH;
    col.pszText = "ID"; col.cx = 60; ListView_InsertColumn(list, 0, &col);
    col.pszText = "Name"; col.cx = 150; ListView_InsertColumn(list, 1, &col);
    col.pszText = "Price"; col.cx = 100; ListView_InsertColumn(list, 2, &col);
    col.pszText = "Qty"; col.cx = 80; ListView_InsertColumn(list, 3, &col);
    int index = 0;
    Product *cur = inv.head;
    char buf[64];
    while (cur) {
        LVITEMA item = {0};
        item.mask = LVIF_TEXT;
        item.iItem = index;
        sprintf(buf, "%d", cur->id);
        item.pszText = buf;
        ListView_InsertItem(list, &item);
        ListView_SetItemText(list, index, 1, cur->name);
        sprintf(buf, "%.2f", cur->price);
        ListView_SetItemText(list, index, 2, buf);
        sprintf(buf, "%d", cur->qty);
        ListView_SetItemText(list, index, 3, buf);
        cur = cur->next;
        index++;
    }
}
void ShowAdminAddProduct(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 28, hwndParent, (HMENU)ID_ADMIN_BACK, NULL, NULL);
    CreateWindowA("static", "Product ID:", WS_VISIBLE|WS_CHILD,
                  100, 90, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 90, 160, 24, hwndParent, (HMENU)501, NULL, NULL);
    CreateWindowA("static", "Name:", WS_VISIBLE|WS_CHILD,
                  100, 130, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 130, 160, 24, hwndParent, (HMENU)502, NULL, NULL);
    CreateWindowA("static", "Price:", WS_VISIBLE|WS_CHILD,
                  100, 170, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 170, 160, 24, hwndParent, (HMENU)503, NULL, NULL);
    CreateWindowA("static", "Quantity:", WS_VISIBLE|WS_CHILD,
                  100, 210, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 210, 160, 24, hwndParent, (HMENU)504, NULL, NULL);
    CreateWindowA("button", "Add Product", WS_VISIBLE|WS_CHILD,
                  180, 260, 160, 32, hwndParent, (HMENU)ID_ADMIN_ADD_CONFIRM, NULL, NULL);
}
void ShowAdminUpdateQty(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 28, hwndParent, (HMENU)ID_ADMIN_BACK, NULL, NULL);
    CreateWindowA("static", "Product ID:", WS_VISIBLE|WS_CHILD,
                  100, 120, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 120, 160, 24, hwndParent, (HMENU)601, NULL, NULL);
    CreateWindowA("static", "New Quantity:", WS_VISIBLE|WS_CHILD,
                  100, 160, 120, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  230, 160, 160, 24, hwndParent, (HMENU)602, NULL, NULL);
    CreateWindowA("button", "Update Qty", WS_VISIBLE|WS_CHILD,
                  200, 210, 140, 32, hwndParent, (HMENU)ID_ADMIN_UPDATE_CONFIRM, NULL, NULL);
}
void ShowAdminRemoveProduct(HWND hwndParent) {
    ClearChildren(hwndParent);
    CreateWindowA("button", "Back", WS_VISIBLE|WS_CHILD,
                  20, 20, 60, 28, hwndParent, (HMENU)ID_ADMIN_BACK, NULL, NULL);
    CreateWindowA("static", "Product ID to Remove:", WS_VISIBLE|WS_CHILD,
                  100, 140, 160, 24, hwndParent, NULL, NULL, NULL);
    CreateWindowA("edit", "", WS_VISIBLE|WS_CHILD|WS_BORDER,
                  260, 140, 130, 24, hwndParent, (HMENU)701, NULL, NULL);
    CreateWindowA("button", "Remove", WS_VISIBLE|WS_CHILD,
                  200, 190, 140, 32, hwndParent, (HMENU)ID_ADMIN_REMOVE_CONFIRM, NULL, NULL);
}
LRESULT CALLBACK MainProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static char u[128], p[128];
    switch(msg) {
    case WM_CREATE:
        ShowMainMenu(hwnd);
        break;
    case WM_COMMAND: {
        WORD id = LOWORD(wParam);
        if(id == ID_BTN_USER) { ShowLogin(hwnd, 0); return 0; }
        if(id == ID_BTN_ADMIN) { ShowLogin(hwnd, 1); return 0; }
        if(id == ID_BTN_BACK) {
            if(strlen(g_username) == 0) ShowMainMenu(hwnd);
            else { if(g_isAdmin) ShowAdminDash(hwnd); else ShowUserDash(hwnd); }
            return 0;
        }
        if(id == ID_BTN_LOGIN || id == ID_BTN_SIGNUP) {
            GetWindowTextA(GetDlgItem(hwnd, ID_EDT_USER), u, sizeof(u));
            GetWindowTextA(GetDlgItem(hwnd, ID_EDT_PASS), p, sizeof(p));
            trim_crlf(u);
            trim_crlf(p);
            if(strlen(u)==0 || strlen(p)==0) {
                MessageBoxA(hwnd, "Enter username & password!", "Error", MB_OK);
                return 0;
            }
            const char *file = g_isAdmin ? ADMIN_FILE : USER_FILE;
            if(id == ID_BTN_SIGNUP) {
                if(username_exists_in_file(USER_FILE, u) || username_exists_in_file(ADMIN_FILE, u)) {
                    MessageBoxA(hwnd, "Username already exists!", "Error", MB_OK);
                    return 0;
                }
                FILE *f = fopen(file, "a+");
                if(!f) { MessageBoxA(hwnd, "Cannot open file for signup.", "Error", MB_OK); return 0; }
                fprintf(f, "%s %s\n", u, p);
                fflush(f);
                fclose(f);
                MessageBoxA(hwnd, "Signup successful! You can now login.", "Success", MB_OK);
                return 0;
            }
            FILE *fr = fopen(file, "r");
            int ok = 0;
            if(fr) {
                char ru[128], rp[128];
                while(fscanf(fr, "%127s %127s", ru, rp) == 2) {
                    trim_crlf(ru);
                    trim_crlf(rp);
                    if(strcmp(ru, u) == 0 && strcmp(rp, p) == 0) { ok = 1; break; }
                }
                fclose(fr);
            } else {
                MessageBoxA(hwnd, "No users registered yet. Please sign up first.", "Error", MB_OK);
                return 0;
            }
            if(!ok) { MessageBoxA(hwnd, "Invalid username or password!", "Error", MB_OK); return 0; }
            strncpy(g_username, u, sizeof(g_username)-1);
            g_username[sizeof(g_username)-1] = '\0';
            if(g_isAdmin) ShowAdminDash(hwnd); else ShowUserDash(hwnd);
            return 0;
        }
        if(!g_isAdmin) {
            if(id == ID_BACK_BROWSE || id == ID_BACK_CART || id == ID_BACK_HISTORY || id == ID_BACK_ADD) {
                ShowUserDash(hwnd);
                return 0;
            }
        } else {
            if(id == ID_ADMIN_BACK) { ShowAdminDash(hwnd); return 0; }
        }
        if(id == ID_U_BROWSE) { ShowProductList(hwnd); return 0; }
        if(id == ID_U_ADD) { ShowAddToCartWindow(hwnd); return 0; }
        if(id == ID_U_VIEWCART) { ShowCartWindow(hwnd); return 0; }
        if(id == ID_U_HISTORY) { ShowHistoryWindow(hwnd); return 0; }
        if(id == ID_ADD_CONFIRM) {
            char pid[32], qty[32];
            GetWindowTextA(GetDlgItem(hwnd,201), pid, sizeof(pid));
            GetWindowTextA(GetDlgItem(hwnd,202), qty, sizeof(qty));
            int idp = atoi(pid);
            int q = atoi(qty);
            Product *pr = find_product_by_id(&inv, idp);
            if(!pr) { MessageBoxA(hwnd,"Invalid Product ID!","Error",MB_OK); return 0; }
            if(q <= 0 || q > pr->qty) { MessageBoxA(hwnd,"Invalid quantity!","Error",MB_OK); return 0; }
            add_to_cart(&cart, idp, q);
            MessageBoxA(hwnd,"Added to cart!","Success",MB_OK);
            ShowUserDash(hwnd);
            return 0;
        }
        if(id == ID_U_CHECKOUT) {
            if(cart.head==NULL) { MessageBoxA(hwnd,"Cart empty!","Error",MB_OK); return 0; }
            print_bill(&cart, &inv);
            save_order_to_file(g_username, &cart, &inv);
            CartItem *c = cart.head;
            while(c) {
                Product *pp = find_product_by_id(&inv, c->product_id);
                if(pp) pp->qty -= c->qty;
                c = c->next;
            }
            clear_cart(&cart);
            MessageBoxA(hwnd,"Checkout done!","OK",MB_OK);
            ShowUserDash(hwnd);
            return 0;
        }
        if(id == ID_U_LOGOUT) {
            strcpy(g_username,"");
            g_isAdmin = 0;
            ShowMainMenu(hwnd);
            return 0;
        }
        if(id == ID_ADMIN_LIST) { ShowAdminListProducts(hwnd); return 0; }
        if(id == ID_ADMIN_ADD) { ShowAdminAddProduct(hwnd); return 0; }
        if(id == ID_ADMIN_UPDATE) { ShowAdminUpdateQty(hwnd); return 0; }
        if(id == ID_ADMIN_REMOVE) { ShowAdminRemoveProduct(hwnd); return 0; }
        if(id == ID_ADMIN_ADD_CONFIRM) {
            char idc[32], name[128], price[32], qty[32];
            GetWindowTextA(GetDlgItem(hwnd,501), idc, sizeof(idc));
            GetWindowTextA(GetDlgItem(hwnd,502), name, sizeof(name));
            GetWindowTextA(GetDlgItem(hwnd,503), price, sizeof(price));
            GetWindowTextA(GetDlgItem(hwnd,504), qty, sizeof(qty));
            Product *pnew = (Product*)malloc(sizeof(Product));
            if(!pnew) { MessageBoxA(hwnd,"Memory error","Error",MB_OK); return 0; }
            pnew->id = atoi(idc);
            strncpy(pnew->name, name, sizeof(pnew->name)-1);
            pnew->name[sizeof(pnew->name)-1] = '\0';
            pnew->price = (float)atof(price);
            pnew->qty = atoi(qty);
            add_product(&inv, pnew);
            MessageBoxA(hwnd,"Product added!","OK",MB_OK);
            ShowAdminDash(hwnd);
            return 0;
        }
        if(id == ID_ADMIN_UPDATE_CONFIRM) {
            char idc[32], qtyc[32];
            GetWindowTextA(GetDlgItem(hwnd,601), idc, sizeof(idc));
            GetWindowTextA(GetDlgItem(hwnd,602), qtyc, sizeof(qtyc));
            Product *p = find_product_by_id(&inv, atoi(idc));
            if(p) {
                p->qty = atoi(qtyc);
                MessageBoxA(hwnd,"Qty updated!","OK",MB_OK);
                ShowAdminDash(hwnd);
            } else {
                MessageBoxA(hwnd,"No such ID","Error",MB_OK);
            }
            return 0;
        }
        if(id == ID_ADMIN_REMOVE_CONFIRM) {
            char idc[32];
            GetWindowTextA(GetDlgItem(hwnd,701), idc, sizeof(idc));
            remove_product(&inv, atoi(idc));
            MessageBoxA(hwnd,"Removed!","OK",MB_OK);
            ShowAdminDash(hwnd);
            return 0;
        }
        if(id == ID_ADMIN_SAVE) {
            save_inventory_to_file(&inv, "products.txt");
            MessageBoxA(hwnd,"Saved!","OK",MB_OK);
            ShowMainMenu(hwnd);
            return 0;
        }
        if(id == ID_ADMIN_LOGOUT) {
            strcpy(g_username,"");
            g_isAdmin = 0;
            ShowMainMenu(hwnd);
            return 0;
        }
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
