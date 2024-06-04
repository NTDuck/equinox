#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iosfwd>   // https://stackoverflow.com/questions/373142/what-techniques-can-be-used-to-speed-up-c-compilation-times
#include <stdexcept>

#include <memory>
#include <functional>
#include <type_traits>
#include <string_view>
#include <tuple>

#include <auxiliaries.hpp>


constexpr inline Point operator-(Point const& obj) { return Point({ -obj.x, -obj.y }); }

constexpr inline Point operator+(Point const& lhs, Point const& rhs) { return Point({ lhs.x + rhs.x, lhs.y + rhs.y }); }
constexpr inline Point operator+(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x + rhs, lhs.y + rhs }); }
constexpr inline Point operator+(int lhs, Point const& rhs) { return rhs + lhs; }
constexpr inline Point operator-(Point const& lhs, Point const& rhs) { return lhs + -rhs; }
constexpr inline Point operator-(Point const& lhs, std::int32_t rhs) { return lhs + -rhs; }
constexpr inline Point operator*(Point const& lhs, Point const& rhs) { return Point({ lhs.x * rhs.x, lhs.y * rhs.y }); }   // Dot product
constexpr inline Point operator*(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x * rhs, lhs.y * rhs }); }
constexpr inline Point operator/(Point const& lhs, Point const& rhs) { return Point({ lhs.x / rhs.x, lhs.y / rhs.y }); }
constexpr inline Point operator/(Point const& lhs, std::int32_t rhs) { return Point({ lhs.x / rhs, lhs.y / rhs }); }

constexpr inline Point& operator++(Point& obj) { obj = obj + 1; return obj; }
constexpr inline Point operator++(Point& obj, std::int32_t) { auto old = obj; ++obj; return old; }
constexpr inline Point& operator--(Point& obj) { obj = obj - 1; return obj; }
constexpr inline Point operator--(Point& obj, std::int32_t) { auto old = obj; --obj; return old; }
constexpr inline Point& operator+=(Point& lhs, Point const& rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator+=(Point& lhs, std::int32_t rhs) { lhs = lhs + rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, Point const& rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator-=(Point& lhs, std::int32_t rhs) { lhs = lhs - rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, Point const& rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator*=(Point& lhs, std::int32_t rhs) { lhs = lhs * rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, Point const& rhs) { lhs = lhs / rhs; return lhs; }
constexpr inline Point& operator/=(Point& lhs, std::int32_t rhs) { lhs = lhs / rhs; return lhs; }

constexpr inline bool operator==(Point const& lhs, Point const& rhs) noexcept { return lhs.x == rhs.x && lhs.y == rhs.y; }
constexpr inline bool operator!=(Point const& lhs, Point const& rhs) noexcept { return !(lhs == rhs); }
constexpr inline bool operator<(Point const& lhs, Point const& rhs) noexcept { return lhs.y == rhs.y ? lhs.y < rhs.y : lhs.x < rhs.x; }   // Simulates rendering order
constexpr inline bool operator>(Point const& lhs, Point const& rhs) noexcept { return rhs < lhs; }
constexpr inline bool operator<=(Point const& lhs, Point const& rhs) noexcept { return !(lhs > rhs); }
constexpr inline bool operator>=(Point const& lhs, Point const& rhs) noexcept { return !(lhs < rhs); }

constexpr inline bool operator==(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x == rhs && lhs.y == rhs; }
constexpr inline bool operator!=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs == rhs); }
constexpr inline bool operator<(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x < rhs && lhs.y < rhs; }   // Simulates rendering order
constexpr inline bool operator>(Point const& lhs, std::int32_t rhs) noexcept { return lhs.x > rhs && lhs.y > rhs; }
constexpr inline bool operator<=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs > rhs); }
constexpr inline bool operator>=(Point const& lhs, std::int32_t rhs) noexcept { return !(lhs < rhs); }

std::ostream& operator<<(std::ostream&, Point const&);
std::ostream& operator<<(std::ostream&, Rect const&);
std::ostream& operator<<(std::ostream&, Color const&);

namespace utility {
    /**
     * @brief Enables string concatenation at compile-time.
     * @see https://stackoverflow.com/questions/38955940/how-to-concatenate-static-strings-at-compile-time
    */
    template <typename CharType, std::basic_string_view<CharType> const&... Strs>
    struct StaticBasicString {
    private:
        // Concatenate all strings into an `std::array<char>`
        static constexpr inline auto MakeBuffer() noexcept {
            constexpr std::size_t bufferSize = (Strs.size() + ...);
            std::array<CharType, bufferSize + 1> buffer{};

            auto appendToBuffer = [pos = 0, &buffer](auto const& s) mutable { for (auto c : s) buffer[pos++] = c; };
            
            (appendToBuffer(Strs), ...);
            buffer[bufferSize] = 0;
            
            return buffer;
        }

        static constexpr inline auto mBuffer = MakeBuffer();   // Static storage
        
    public:
        static constexpr std::basic_string_view<CharType> value { mBuffer.data(), mBuffer.size() - 1 };
    };

    template <std::string_view const&... Strs>
    using StaticString = StaticBasicString<char, Strs...>;

    template <typename K, typename V>
    struct StaticPairTtoT {
        StaticPairTtoT() = delete;
        ~StaticPairTtoT() = delete;

        using Key = K;
        using Value = V;
    };

    template <decltype(auto) K, decltype(auto) V>
    struct StaticPairVtoV {
        StaticPairVtoV() = delete;
        ~StaticPairVtoV() = delete;

        static constexpr decltype(auto) Key = K;
        static constexpr decltype(auto) Value = V;
    };

    template <typename K, decltype(auto) V>
    struct StaticPairTtoV {
        StaticPairTtoV() = delete;
        ~StaticPairTtoV() = delete;

        using Key = K;
        static constexpr decltype(auto) Value = V;
    };

    template <decltype(auto) K, typename V>
    struct StaticPairVtoT {
        StaticPairVtoT() = delete;
        ~StaticPairVtoT() = delete;

        static constexpr decltype(auto) Key = K;
        using Value = V;
    };

    /**
     * @brief A compile-time map that supports both typename and values.
     * 
     * @tparam Pairs Can be either `StaticPairTtoT`, `StaticPairVtoV`, `StaticPairTtoV`, or `StaticPairVtoT`. Do note that using more than one type may result in undefined behaviour.
    */
    template <typename... Pairs>
    class StaticMap {
        template <typename T, typename U = std::void_t<>>
        struct HasNestedTypeKey : std::false_type {
            static constexpr inline bool Result = value;
        };

        template <typename T>
        struct HasNestedTypeKey<T, std::void_t<typename T::Key>> : std::true_type {
            static constexpr inline bool Result = value;
        };

        template <typename T, typename U = std::void_t<>>
        struct HasNestedTypeValue : std::false_type {
            static constexpr inline bool Result = value;
        };

        template <typename T>
        struct HasNestedTypeValue<T, std::void_t<typename T::Value>> : std::true_type {
            static constexpr inline bool Result = value;
        };

        template <typename Pair, bool = HasNestedTypeKey<Pair>::Result, bool = HasNestedTypeValue<Pair>::Result>
        struct QueryResult;

        template <typename Pair>
        struct QueryResult<Pair, true, true> {
            using Key = typename Pair::Key;
            using Value = typename Pair::Value;
        };

        template <typename Pair>
        struct QueryResult<Pair, false, false> {
            static constexpr auto Key = Pair::Key;
            static constexpr auto Value = Pair::Value;
        };

        template <typename Pair>
        struct QueryResult<Pair, true, false> {
            using Key = typename Pair::Key;
            static constexpr auto Value = Pair::Value;
        };

        template <typename Pair>
        struct QueryResult<Pair, false, true> {
            static constexpr auto Key = Pair::Key;
            using Value = typename Pair::Value;
        };    

        template <typename K, typename FirstPair, typename... OtherPairs>
        struct QueryTypeImpl {
            using Result = std::conditional_t<
                HasNestedTypeKey<FirstPair>::Result && std::is_same_v<K, typename FirstPair::Key>,
                QueryResult<FirstPair>, typename QueryTypeImpl<K, OtherPairs...>::Result
            >;
        };

        template <typename K, typename LastPair>
        struct QueryTypeImpl<K, LastPair> {
            using Result = QueryResult<LastPair>;
        };

        template <decltype(auto) K, typename FirstPair, typename... OtherPairs>
        struct QueryValueImpl {
            using Result = std::conditional_t<
                !HasNestedTypeKey<FirstPair>::Result && std::is_same_v<decltype(K), decltype(FirstPair::Key)> && K == FirstPair::Key,
                QueryResult<FirstPair>, typename QueryValueImpl<K, OtherPairs...>::Result
            >;
        };

        template <decltype(auto) K, typename LastPair>
        struct QueryValueImpl<K, LastPair> {
            using Result = QueryResult<LastPair>;
        };

    public:
        StaticMap() = delete;
        ~StaticMap() = delete;

        template <typename K>
        struct QueryType {
            using Result = typename QueryTypeImpl<K, Pairs...>::Result;
        };

        template <decltype(auto) K>
        struct QueryValue {
            using Result = typename QueryValueImpl<K, Pairs...>::Result;
        };
    };

    template <typename T>
    class Singleton {
    public:
        template <typename... Args>
        static inline T& Get(Args&&... args) {
            static T instance(std::forward<Args>(args)...);
            return instance;
        }

        Singleton(Singleton const&) = delete;
        Singleton& operator=(Singleton const&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton&&) = delete;

    protected:
        constexpr inline explicit Singleton() = default;
        inline virtual ~Singleton() = default;
    };

    template <typename T, decltype(auto) Allocator, decltype(auto) Deleter, typename Container = std::shared_ptr<T>>
    class SmartPointer {
    public:
        constexpr inline SmartPointer() noexcept {}
        constexpr inline SmartPointer(std::nullptr_t) noexcept {}
        ~SmartPointer();

        template <typename... Args>
        SmartPointer(Args&&...);

        template <typename... Args>
        void Reset(Args&&...);
        void Reset() noexcept;

        T* Get() const noexcept;
        T& operator*() const noexcept;
        T* operator->() const noexcept;

        explicit operator bool() const noexcept;

    private:
        Container mContainer;
    };

    template <typename ObjectID, ObjectID N, typename Tuple>
    class StructOfArray;

    /**
     * @brief Struct of Array Implementation.
     * 
     * @tparam ObjectID The type used as identifier for objects, preferably `std::uint32_t`.
     * @tparam N The maximum number of objects.
     * @tparam Args The member types.
     * 
     * @see https://medium.com/@savas/nomad-game-engine-part-4-4-soa-implementation-1c8e733dc0eb
     * @see https://austinmorlan.com/posts/entity_component_system/
    */
    template <typename ObjectID, ObjectID N, typename... Args>
    class StructOfArray<ObjectID, N, std::tuple<Args...>> {
    public:
        /**
         * @note For example, the `Object` equivalence of `struct P { int foo; std::string bar; }` is `std::tuple<int, std::string>`.
        */
        using Object = std::tuple<Args...>;

        /**
         * @note For example, the `Member<0>` equivalence of `struct P { int foo; std::string bar; }` is `int`, `Member<1>` is `std::string` and so on.
        */
        template <std::size_t I>
        using Member = std::tuple_element_t<I, Object>;

        static constexpr inline std::size_t GetMemberCount() {
            static_assert(std::tuple_size_v<Object> == sizeof...(Args));
            return std::tuple_size_v<Object>;
        }

        std::size_t GetObjectCount() const noexcept;

        void Insert(ObjectID, Object const&);
        void Erase(ObjectID);

        template <std::size_t I>
        Member<I>& Get(ObjectID);

    protected:
        template <std::size_t I = 0>
        void InsertObject(ObjectID, Object const&);

        template <std::size_t I = 0>
        void InsertObject(ObjectID, ObjectID);

        std::tuple<std::array<Args, N>...> mBuffer;
        std::unordered_map<ObjectID, std::size_t> mObjectIDToMemberIndexMap{};
        std::unordered_map<std::size_t, ObjectID> mMemberIndexToObjectIDMap{};
        ObjectID mObjectCount{};
    };

    /**
     * @see https://medium.com/@savas/template-metaprogramming-compile-time-loops-over-class-methods-a243dc346122
    */
    class For;

    /**
     * @brief Optimal parameter passing technique of type `T`. Selecting `OptimalParamType<T>` over `T` always result in marginally increased performance.
     * @note `OptimalParamType<T>` is implicitly convertible to `T`.
    */

    template <typename T>
    std::string_view GetRawTypeName();

    /**
     * @note Requires header `<cxxabi.h>`, which might not be present on all compilers.
    */
    template <typename T>
    std::string_view GetTypeName();

    struct GetColor {
        constexpr inline Color operator()(std::string_view hexString) const {
            FormatHexString(hexString);
            return HexIntToRGBA(HexStringToHexInt(hexString));
        }

        static constexpr char kPrefix = '#';
        static constexpr char kAutofill = 'f';

    private:
        static constexpr inline void FormatHexString(std::string_view& hexString) {
            if (hexString.front() == kPrefix)
                hexString.remove_prefix(1);
            
            if (hexString.size() > 8)
                hexString = hexString.substr(0, 8);
        }

        static constexpr inline std::uint8_t HexCharToUint8(char hexChar) {
            if ('0' <= hexChar && hexChar <= '9')
                return hexChar - '0';
            if ('A' <= hexChar && hexChar <= 'F')
                return hexChar - 'A' + 10;
            if ('a' <= hexChar && hexChar <= 'f')
                return hexChar - 'a' + 10;

            return kAutofill;
        }

        static constexpr inline std::uint32_t HexStringToHexInt(std::string_view hexString) {
            std::uint32_t value = 0;
            std::uint32_t pos = 8 - hexString.size();

            for (auto chr : hexString)
                value = value * 16 + HexCharToUint8(chr);

            while (pos--)
                value = value * 16 + HexCharToUint8(kAutofill);

            return value;
        }

        static constexpr inline Color HexIntToRGBA(std::uint32_t hexInt) {
            std::uint8_t r = (hexInt >> 24) & 0xff;
            std::uint8_t g = (hexInt >> 16) & 0xff;
            std::uint8_t b = (hexInt >> 8) & 0xff;
            std::uint8_t a = hexInt & 0xff;

            return { r, g, b, a };
        }
    };
}

namespace logger {
    enum class Level {
        kEmergency,
        kAlert,
        kCritical,
        kError,
        kWarning,
        kNotice,
        kInformational,
        kDebug,
    };

    static constexpr std::array<std::string_view, static_cast<std::size_t>(Level::kDebug) - static_cast<std::size_t>(Level::kEmergency) + 1> kPrefixes {
        "emerg", "alert", "crit", "error", "warn", "notice", "info", "debug",
    };
    static constexpr std::string_view kDelimiter = " : ";

    template <Level L, std::string_view const&... Strs>
    struct formatter {
    private:
        static constexpr auto mPrefix = kPrefixes[static_cast<std::size_t>(L)];

    public:
        static constexpr inline auto value = utility::StaticString<mPrefix, kDelimiter, Strs...>::value;
    };
}


#include <utilities/smart-pointer.tpp>
#include <utilities/struct-of-array.tpp>
#include <utilities/type-name.tpp>

#endif