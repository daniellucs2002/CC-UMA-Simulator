class CacheSet;

class CacheLine {
    friend class CacheSet;
private:
    bool is_valid;
    unsigned int tag;
    bool is_dirty;
public:
    CacheLine() : is_valid(false), tag(0), is_dirty(false) { }
};
