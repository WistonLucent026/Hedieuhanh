#include <iostream>

using namespace std;

// Hàm cơ bản tự viết để kiểm tra một trang đã có trong khung trang hay chưa
bool KiemTraTonTai(int trang, int khungTrang[], int soKhungTrangHienTai) {
    for (int i = 0; i < soKhungTrangHienTai; ++i) {
        if (khungTrang[i] == trang) {
            return true; // Đã có trong bộ nhớ
        }
    }
    return false; // Chưa có trong bộ nhớ
}

// 1. Thuật toán FIFO (Vào trước ra trước)
int TinhLoiTrangFIFO(int chuoiTrang[], int soLuongTrang, int soKhungTrang) {
    int khungTrang[100]; // Mảng lưu các trang đang có trong bộ nhớ
    int soKhungTrangHienTai = 0;
    int soLoiTrang = 0;
    int viTriThayThe = 0; // Con trỏ vòng lặp để biết trang nào vào sớm nhất

    for (int i = 0; i < soLuongTrang; ++i) {
        int trang = chuoiTrang[i];
        
        // Nếu trang chưa có trong khung (Lỗi trang)
        if (!KiemTraTonTai(trang, khungTrang, soKhungTrangHienTai)) {
            soLoiTrang++;
            
            // Nếu khung trang chưa đầy, thêm thẳng vào
            if (soKhungTrangHienTai < soKhungTrang) {
                khungTrang[soKhungTrangHienTai] = trang;
                soKhungTrangHienTai++;
            } else {
                // Nếu đã đầy, thay thế ở vị trí vào sớm nhất và cuốn chiếu (vòng tròn) con trỏ
                khungTrang[viTriThayThe] = trang;
                viTriThayThe = (viTriThayThe + 1) % soKhungTrang;
            }
        }
    }
    return soLoiTrang;
}

// 2. Thuật toán LRU (Ít sử dụng nhất gần đây)
int TinhLoiTrangLRU(int chuoiTrang[], int soLuongTrang, int soKhungTrang) {
    int khungTrang[100];
    int thoiGianSuDung[100]; // Mảng lưu lại thời điểm (chỉ số i) sử dụng gần nhất của từng khung
    int soKhungTrangHienTai = 0;
    int soLoiTrang = 0;

    for (int i = 0; i < soLuongTrang; ++i) {
        int trang = chuoiTrang[i];
        bool tonTai = false;

        // Tự duyệt mảng để kiểm tra tồn tại và cập nhật thời gian nếu đã có
        for (int j = 0; j < soKhungTrangHienTai; ++j) {
            if (khungTrang[j] == trang) {
                tonTai = true;
                thoiGianSuDung[j] = i; // Đánh dấu là vừa mới được dùng tại bước thứ i
                break;
            }
        }

        // Nếu xảy ra lỗi trang
        if (!tonTai) {
            soLoiTrang++;
            if (soKhungTrangHienTai < soKhungTrang) {
                khungTrang[soKhungTrangHienTai] = trang;
                thoiGianSuDung[soKhungTrangHienTai] = i;
                soKhungTrangHienTai++;
            } else {
                // Tìm trang có thời điểm sử dụng nhỏ nhất (tức là lâu nhất chưa dùng)
                int viTriLRU = 0;
                int thoiGianNhoNhat = thoiGianSuDung[0];
                for (int j = 1; j < soKhungTrang; ++j) {
                    if (thoiGianSuDung[j] < thoiGianNhoNhat) {
                        thoiGianNhoNhat = thoiGianSuDung[j];
                        viTriLRU = j;
                    }
                }
                // Thay thế trang đó
                khungTrang[viTriLRU] = trang;
                thoiGianSuDung[viTriLRU] = i;
            }
        }
    }
    return soLoiTrang;
}

// 3. Thuật toán OPT (Tối ưu)
int TinhLoiTrangOPT(int chuoiTrang[], int soLuongTrang, int soKhungTrang) {
    int khungTrang[100];
    int soKhungTrangHienTai = 0;
    int soLoiTrang = 0;

    for (int i = 0; i < soLuongTrang; ++i) {
        int trang = chuoiTrang[i];
        
        if (!KiemTraTonTai(trang, khungTrang, soKhungTrangHienTai)) {
            soLoiTrang++;
            if (soKhungTrangHienTai < soKhungTrang) {
                khungTrang[soKhungTrangHienTai] = trang;
                soKhungTrangHienTai++;
            } else {
                int viTriXaNhat = -1;
                int chiSoThayThe = -1;

                // Với mỗi trang đang nằm trong RAM, tìm xem bao giờ nó mới được dùng lại
                for (int j = 0; j < soKhungTrang; ++j) {
                    int k;
                    // Duyệt phần còn lại của chuỗi truy cập
                    for (k = i + 1; k < soLuongTrang; ++k) {
                        if (khungTrang[j] == chuoiTrang[k]) {
                            break; // Tìm thấy vị trí xuất hiện tiếp theo
                        }
                    }
                    
                    // Nếu vị trí xuất hiện tiếp theo xa hơn mức kỷ lục hiện tại
                    if (k > viTriXaNhat) {
                        viTriXaNhat = k;
                        chiSoThayThe = j;
                    }
                }
                // Thay thế trang có vị trí xuất hiện lại xa nhất (hoặc không bao giờ xuất hiện nữa)
                khungTrang[chiSoThayThe] = trang;
            }
        }
    }
    return soLoiTrang;
}

int main() {
    int soKhungTrang;
    cout << "Nhap so khung trang (Vi du: 3): ";
    cin >> soKhungTrang;

    int soLuongTrang;
    cout << "Nhap do dai cua chuoi truy cap trang (Vi du: 20): ";
    cin >> soLuongTrang;

    int chuoiTrang[1000]; // Mảng tĩnh chứa chuỗi truy cập trang
    cout << "Nhap chuoi truy cap trang (cac so cach nhau boi dau cach):\n";
    for (int i = 0; i < soLuongTrang; ++i) {
        cin >> chuoiTrang[i];
    }

    cout << "\n---------------- KET QUA ----------------\n";
    cout << "So loi trang theo thuat toan FIFO: " << TinhLoiTrangFIFO(chuoiTrang, soLuongTrang, soKhungTrang) << "\n";
    cout << "So loi trang theo thuat toan LRU:  " << TinhLoiTrangLRU(chuoiTrang, soLuongTrang, soKhungTrang) << "\n";
    cout << "So loi trang theo thuat toan OPT:  " << TinhLoiTrangOPT(chuoiTrang, soLuongTrang, soKhungTrang) << "\n";
    cout << "-----------------------------------------\n";

    return 0;
}
