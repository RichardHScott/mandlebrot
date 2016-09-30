#include <cstdint>
#include <memory>

using namespace std;

class BitmapClass {
    private:


    class Bitmap_Data {
        public:
        uint32_t horizontal_pixels;
        uint32_t vertical_pixels;
        unique_ptr<uint8_t[]> bitmapArray;
        uint32_t bitmapArraySize;

        Bitmap_Data(int horizontal, int vertical, unique_ptr<uint8_t[]>& data) {
            horizontal_pixels = horizontal;
            vertical_pixels = vertical;
            bitmapArraySize = horizontal * vertical;
            bitmapArray = std::move(data);
        }

        ~Bitmap_Data() {
        }
    };

    typedef struct {
        uint16_t header;
        uint32_t file_size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t pixel_data_offset;
    } __attribute__((packed)) Bitmap_File_Header;

    typedef struct {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t number_of_color_planes;
        uint16_t bits_per_pixel;
        uint32_t compression_method;
        uint32_t image_size;
        int32_t horizontal_resolution;
        int32_t vertical_resolution;
        uint32_t number_of_colors_in_color_palette;
        uint32_t number_of_important_colors_used;
    }  __attribute__((packed)) DIB_Header;

    struct Bitmap {
        Bitmap_File_Header *file_header;
        DIB_Header *dib_header;
        unique_ptr<Bitmap_Data> data;

        Bitmap(unique_ptr<Bitmap_Data>& bitmap_data) {
            data = std::move(bitmap_data);
        }
    };

    uint8_t* one_pixel_blur(uint8_t* data, int width, int height);

    unique_ptr<Bitmap_Data> four_times_AA(unique_ptr<uint8_t[]> data, int width, int height);

    int save_bitmap_to_disk(FILE* file);
    int save_bitmap(std::string const & filename);

    unique_ptr<Bitmap> create_bitmap(uint32_t horizontal_dim, uint32_t vertical_dim, unique_ptr<uint8_t[]>& data);

    unique_ptr<Bitmap> bitmap;

    protected:
    public:
        BitmapClass(uint32_t horizontal_dim, uint32_t vertical_dim, unique_ptr<uint8_t[]>& data);
        ~BitmapClass();
        BitmapClass(BitmapClass const &) = delete;
        BitmapClass(BitmapClass&&) = delete;

        void WriteToDisk(std::string const & filename);
};