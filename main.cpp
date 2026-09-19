#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// =================== CÁC GIẢI THUẬT THAY THẾ TRANG ===================

// Giải thuật FIFO (First In First Out)
int GiaiThuat_FIFO(const vector<int>& chuoi_truy_cap, int so_khung) {
    vector<int> khung_trang;
    int so_loi_trang = 0;
    int vi_tri_thay_the = 0;

    for (int trang : chuoi_truy_cap) {
        // Kiểm tra xem trang đã có trong khung chưa
        auto it = find(khung_trang.begin(), khung_trang.end(), trang);
        
        if (it == khung_trang.end()) { // Nếu chưa có -> Lỗi trang
            so_loi_trang++;
            if (khung_trang.size() < so_khung) {
                khung_trang.push_back(trang);
            } else {
                khung_trang[vi_tri_thay_the] = trang;
                vi_tri_thay_the = (vi_tri_thay_the + 1) % so_khung;
            }
        }
    }
    return so_loi_trang;
}

// Giải thuật LRU (Least Recently Used)
int GiaiThuat_LRU(const vector<int>& chuoi_truy_cap, int so_khung) {
    vector<int> khung_trang;
    vector<int> thoi_gian_su_dung;
    int so_loi_trang = 0;
    int thoi_gian = 0;

    for (int trang : chuoi_truy_cap) {
        thoi_gian++;
        auto it = find(khung_trang.begin(), khung_trang.end(), trang);

        if (it == khung_trang.end()) { // Lỗi trang
            so_loi_trang++;
            if (khung_trang.size() < so_khung) {
                khung_trang.push_back(trang);
                thoi_gian_su_dung.push_back(thoi_gian);
            } else {
                // Tìm trang ít được sử dụng nhất
                int vi_tri_lru = distance(thoi_gian_su_dung.begin(), min_element(thoi_gian_su_dung.begin(), thoi_gian_su_dung.end()));
                khung_trang[vi_tri_lru] = trang;
                thoi_gian_su_dung[vi_tri_lru] = thoi_gian;
            }
        } else { // Trúng trang, cập nhật thời gian
            int vi_tri = distance(khung_trang.begin(), it);
            thoi_gian_su_dung[vi_tri] = thoi_gian;
        }
    }
    return so_loi_trang;
}

// Giải thuật OPT (Optimal - Tối ưu)
int GiaiThuat_OPT(const vector<int>& chuoi_truy_cap, int so_khung) {
    vector<int> khung_trang;
    int so_loi_trang = 0;

    for (size_t i = 0; i < chuoi_truy_cap.size(); i++) {
        int trang = chuoi_truy_cap[i];
        auto it = find(khung_trang.begin(), khung_trang.end(), trang);

        if (it == khung_trang.end()) { // Lỗi trang
            so_loi_trang++;
            if (khung_trang.size() < so_khung) {
                khung_trang.push_back(trang);
            } else {
                // Tìm trang sẽ lâu được dùng nhất trong tương lai
                int vi_tri_thay = -1;
                int tuong_lai_xa_nhat = -1;

                for (size_t j = 0; j < khung_trang.size(); j++) {
                    int trang_trong_khung = khung_trang[j];
                    int vi_tri_xuat_hien_tiep = -1;
                    
                    for (size_t k = i + 1; k < chuoi_truy_cap.size(); k++) {
                        if (chuoi_truy_cap[k] == trang_trong_khung) {
                            vi_tri_xuat_hien_tiep = k;
                            break;
                        }
                    }

                    if (vi_tri_xuat_hien_tiep == -1) { // Không bao giờ xuất hiện lại
                        vi_tri_thay = j;
                        break;
                    } else if (vi_tri_xuat_hien_tiep > tuong_lai_xa_nhat) {
                        tuong_lai_xa_nhat = vi_tri_xuat_hien_tiep;
                        vi_tri_thay = j;
                    }
                }
                khung_trang[vi_tri_thay] = trang;
            }
        }
    }
    return so_loi_trang;
}

// =================== GIAO DIỆN ĐỒ HỌA (WinAPI) ===================

HWND hInputChuoi, hInputSoKhung, hButton, hOutput;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_CREATE:
            CreateWindowW(L"Static", L"Chuỗi truy cập (cách nhau bởi khoảng trắng):", WS_VISIBLE | WS_CHILD, 20, 20, 300, 20, hwnd, NULL, NULL, NULL);
            hInputChuoi = CreateWindowW(L"Edit", L"7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 45, 300, 25, hwnd, NULL, NULL, NULL);
            
            CreateWindowW(L"Static", L"Số khung trang (Frames):", WS_VISIBLE | WS_CHILD, 20, 80, 200, 20, hwnd, NULL, NULL, NULL);
            hInputSoKhung = CreateWindowW(L"Edit", L"3", WS_VISIBLE | WS_CHILD | WS_BORDER, 20, 105, 100, 25, hwnd, NULL, NULL, NULL);
            
            hButton = CreateWindowW(L"Button", L"Tính Số Lỗi Trang", WS_VISIBLE | WS_CHILD, 20, 145, 150, 30, hwnd, (HMENU)1, NULL, NULL);
            hOutput = CreateWindowW(L"Static", L"Kết quả sẽ hiển thị ở đây...", WS_VISIBLE | WS_CHILD, 20, 190, 300, 100, hwnd, NULL, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) { // Khi bấm nút
                char bufChuoi[256], bufKhung[10];
                GetWindowTextA(hInputChuoi, bufChuoi, 256);
                GetWindowTextA(hInputSoKhung, bufKhung, 10);

                int so_khung = atoi(bufKhung);
                if (so_khung <= 0) {
                    SetWindowTextW(hOutput, L"Vui lòng nhập số khung trang hợp lệ > 0!");
                    break;
                }

                vector<int> chuoi_truy_cap;
                stringstream ss(bufChuoi);
                int so;
                while (ss >> so) {
                    chuoi_truy_cap.push_back(so);
                }

                int loi_FIFO = GiaiThuat_FIFO(chuoi_truy_cap, so_khung);
                int loi_LRU = GiaiThuat_LRU(chuoi_truy_cap, so_khung);
                int loi_OPT = GiaiThuat_OPT(chuoi_truy_cap, so_khung);

                char ketQua[256];
                sprintf(ketQua, "Số lỗi trang (Page Faults):\n- Giải thuật FIFO: %d\n- Giải thuật LRU: %d\n- Giải thuật OPT: %d", loi_FIFO, loi_LRU, loi_OPT);
                
                // Chuyển char* sang wchar_t* để in ra giao diện chuẩn
                wchar_t wKetQua[256];
                MultiByteToWideChar(CP_UTF8, 0, ketQua, -1, wKetQua, 256);
                SetWindowTextW(hOutput, wKetQua);
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
    wc.lpszClassName = L"GiaoDienBTLHienThucTrang";
    wc.lpfnWndProc = WndProc;

    if (!RegisterClassW(&wc)) return -1;

    HWND hwnd = CreateWindowW(L"GiaoDienBTLHienThucTrang", L"Minh Họa Thuật Toán Thay Thế Trang - C++", 
                              WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 400, 350, 
                              NULL, NULL, hInstance, NULL);

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
