#include <algorithm>
#include <fstream>
#include <list>
#include <optional>
#include <print>
#include <ranges>
#include <vector>

struct File {
    size_t id;
    size_t location;
    size_t size;

    friend bool operator<(const File& f1, const File& f2){
        return f1.location < f2.location;
    }

    size_t getChecksum(){
        size_t total = 0;
        for(auto i = location; i < location+size; ++i){
            total += i*id;
        }
        return total;
    }
};

struct Empty {
    size_t location;
    size_t size;
    
    friend bool operator<(const Empty& e1, const Empty& e2){
        return e1.location < e2.location;
    }
};
class BlockLayout {
public:
    BlockLayout(const std::string& line){
        size_t position = 0;
        size_t fileId = 0;
        bool isFile = true;
        for(char c: line){
            size_t size = c - '0';

            if(isFile){
                for(auto i = 0u; i < size; ++i){
                    blocks.emplace_back(File{fileId, position, size});
                }
                fileId++;
            }
            else {
                for(auto i = 0u; i < size; ++i){
                    blocks.emplace_back(std::nullopt);
                }
            }
            position += size;
            isFile = !isFile;
        }
    }

    void compact() {
        auto openSpace = blocks.begin();
        auto fileToMove = blocks.end() - 1;
        while(openSpace < fileToMove){
            while(openSpace->has_value() && openSpace < fileToMove) openSpace++;
            while(!fileToMove->has_value() && openSpace < fileToMove) fileToMove--;

            *openSpace = *fileToMove;
            *fileToMove = {};
            // position is not used for this
        }
    }

    size_t getChecksum() const {
        auto fileChecksums = std::views::enumerate(blocks) | 
            std::views::transform([](auto b){ auto [i, file] = b; return file.has_value() ? (i*file.value().id) : 0;});
        return std::ranges::fold_left(fileChecksums, 0u, std::plus<>{});
    }

private:
    std::vector<std::optional<File>> blocks;
};
class Layout {
public:
    
    Layout(const std::string& line){
        bool isFile = true;
        size_t position = 0;
        size_t fileId = 0;
        for(char c: line){
            size_t size = c - '0';
            if(isFile){
                files.emplace_back(fileId++, position, size);
            }
            else {
                if(size != 0){
                    empty.emplace_back(position, size);
                }
            }
            position += size;
            isFile = !isFile;
        }
    }

    void compact(){
        for(auto& f: std::views::reverse(files)){
            auto firstEmptySpace = std::ranges::find_if(empty, [&f](auto e) { return e.location < f.location && e.size >= f.size; });
            if(firstEmptySpace != empty.end()){
                Empty newEmpty{f.location, f.size};
                auto placeToInsert = std::ranges::lower_bound(empty, newEmpty, std::less{});
                auto inserted = empty.insert(placeToInsert, newEmpty);
                while(placeToInsert != empty.end() && newEmpty.location + newEmpty.size == placeToInsert->location){
                    auto oldEmpty = *placeToInsert;
                    empty.erase(placeToInsert);
                    newEmpty.size += oldEmpty.size;
                    placeToInsert = std::next(inserted);
                }

                f.location = firstEmptySpace->location;
                firstEmptySpace->size -= f.size;
                firstEmptySpace->location += f.size;
                if(firstEmptySpace->size == 0){
                    empty.erase(firstEmptySpace);
                }
                
            }

        }

    }

    void print() const {
        auto fileIter = files.begin();
        
        auto emptyIter = empty.begin();
        while(fileIter != files.end() ){
            if(emptyIter != empty.end() && emptyIter->location < fileIter->location){
                 for(auto i = 0u; i < emptyIter->size; ++i){
                    std::print(".");
                 } 
                 emptyIter++;
            }
            if(emptyIter == empty.end() || fileIter->location < emptyIter->location ){
                for(auto i = 0u; i < fileIter->size; ++i){
                    std::print("{}", (char)(fileIter->id + '0'));
                } 
                fileIter++;
            }
        }
        std::println();
    }

    size_t getChecksum() const {
        auto fileChecksums = std::views::transform(files, [](auto f){ return f.getChecksum(); });
        return std::ranges::fold_left(fileChecksums, 0u, std::plus<>{});

    }


private:

    std::list<File> files;
    std::list<Empty> empty;
};

int main(){

    std::ifstream in{"input/day9.txt"};
    std::string line;
    in >> line;


    BlockLayout l(line);
    l.compact();
    std::println("The block layout checksum is {}", l.getChecksum());

    Layout layout(line);
    layout.compact();
    std::println("The block layout checksum is {}", layout.getChecksum());


    return 0;
}
