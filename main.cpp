#include <windows.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Khai báo ID cho các thành phần giao diện
#define ID_NHAP_CHUOI 1
#define ID_NHAP_KHUNG_TRANG 2
#define ID_NUT_CHAY 3
#define ID_KET_QUA 4

// --- PHẦN LOGIC THUẬT TOÁN HỆ ĐIỀU HÀNH ---

// Hàm tính số lỗi trang theo giải thuật FIFO
int tinhLoiTrangFIFO(const std::vector<int>& chuoiTruyCap, int soKhungTrang) {
    std::vector<int> khungTrang;
    int soLoiTrang = 0;
    int viTriThayThe = 0;

    for (int trang : chuoiTruyCap) {
        // Kiểm tra trang đã có trong khung chưa
        auto it = std::find(khungTrang.begin(), khungTrang.end(), trang);
        
        if (it == khungTrang.end()) { // Nếu chưa có -> Lỗi trang
            soLoiTrang++;
            if (khungTrang.size() < (size_t)soKhungTrang) {
                khungTrang.push_back(trang);
            } else {
                khungTrang[viTriThayThe] = trang;
                viTriThayThe = (viTriThayThe + 1) % soKhungTrang;
            }
        }
    }
    return soLoiTrang;
}

// Hàm tính số lỗi trang theo giải thuật LRU (Ít sử dụng gần đây nhất)
int tinhLoiTrangLRU(const std::vector<int>& chuoiTruyCap, int soKhungTrang) {
    std::vector<int> khungTrang;
    int soLoiTrang = 0;

    for (int trang : chuoiTruyCap) {
        auto it = std::find(khungTrang.begin(), khungTrang.end(), trang);
        
        if (it == khungTrang.end()) { // Chưa có trong khung
            soLoiTrang++;
            if (khungTrang.size() < (size_t)soKhungTrang) {
                khungTrang.push_back(trang);
            } else {
                // Xóa phần tử đầu tiên (ít dùng nhất) và thêm vào cuối
                khungTrang.erase(khungTrang.begin());
                khungTrang.push_back(trang);
            }
        } else {
            // Nếu đã có, đưa trang đó xuống cuối mảng (đánh dấu là mới dùng)
            khungTrang.erase(it);
            khungTrang.push_back(trang);
        }
    }
    return soLoiTrang;
}

// --- PHẦN GIAO DIỆN ĐỒ HỌA (WIN32 API) ---

HWND hNhapChuoi, hNhapKhungTrang, hKetQua;

LRESULT CALLBACK XuLySuKien(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            // Tạo các nhãn (Labels)
            CreateWindowW(L"STATIC", L"Nhập chuỗi truy cập trang (cách nhau bởi khoảng trắng):", 
                WS_VISIBLE | WS_CHILD, 20, 20, 450, 20, hwnd, NULL, NULL, NULL);
            CreateWindowW(L"STATIC", L"Nhập số lượng khung trang:", 
                WS_VISIBLE | WS_CHILD, 20, 90, 450, 20, hwnd, NULL, NULL, NULL);
            CreateWindowW(L"STATIC", L"Kết quả số lỗi trang:", 
                WS_VISIBLE | WS_CHILD, 20, 160, 450, 20, hwnd, NULL, NULL, NULL);

            // Tạo ô nhập liệu
            hNhapChuoi = CreateWindowW(L"EDIT", L"7 0 1 2 0 3 0 4", 
                WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 50, 450, 25, hwnd, (HMENU)ID_NHAP_CHUOI, NULL, NULL);
            hNhapKhungTrang = CreateWindowW(L"EDIT", L"3", 
                WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 120, 100, 25, hwnd, (HMENU)ID_NHAP_KHUNG_TRANG, NULL, NULL);
            
            // Tạo nút bấm
            CreateWindowW(L"BUTTON", L"Tính Số Lỗi Trang", 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 150, 120, 150, 25, hwnd, (HMENU)ID_NUT_CHAY, NULL, NULL);

            // Tạo ô hiện kết quả
            hKetQua = CreateWindowW(L"EDIT", L"", 
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY, 
                20, 190, 450, 100, hwnd, (HMENU)ID_KET_QUA, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_NUT_CHAY) {
                wchar_t chuoiTxt[256], khungTxt[10];
                GetWindowTextW(hNhapChuoi, chuoiTxt, 256);
                GetWindowTextW(hNhapKhungTrang, khungTxt, 10);

                // Chuyển dữ liệu Text sang số liệu
                std::wstring wsChuoi(chuoiTxt);
                std::string sChuoi(wsChuoi.begin(), wsChuoi.end());
                std::stringstream ss(sChuoi);
                std::vector<int> chuoiTruyCap;
                int trang;
                while (ss >> trang) {
                    chuoiTruyCap.push_back(trang);
                }

                int soKhungTrang = _wtoi(khungTxt);

                // Gọi hàm tính toán
                int loiFIFO = tinhLoiTrangFIFO(chuoiTruyCap, soKhungTrang);
                int loiLRU = tinhLoiTrangLRU(chuoiTruyCap, soKhungTrang);

                // In ra kết quả
                std::wstring ketQuaXuat = L"Thuật toán FIFO: " + std::to_wstring(loiFIFO) + L" lỗi trang.\r\n"
                                        + L"Thuật toán LRU: " + std::to_wstring(loiLRU) + L" lỗi trang.";
                SetWindowTextW(hKetQua, ketQuaXuat.c_str());
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInstance;
    wc.lpszClassName = L"CuaSoBaiTapLon";
    wc.lpfnWndProc = XuLySuKien;

    if (!RegisterClassW(&wc)) return -1;

    HWND hwnd = CreateWindowW(L"CuaSoBaiTapLon", L"Mô Phỏng Giải Thuật Thay Thế Trang (BTL Hệ Điều Hành)", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 350, NULL, NULL, hInstance, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
