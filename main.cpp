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
