#ifndef DPSG_FLAG_HPP
#define DPSG_FLAG_HPP


#define DPSG_PP_NARG(...) \
	DPSG_PP_NARG_(__VA_ARGS__, DPSG_PP_RSEQ_N())

#define DPSG_PP_NARG_(...) \
	DPSG_PP_ARG_N(__VA_ARGS__)

#define DPSG_PP_ARG_N( \
		_1, _2, _3, _4, _5, _6, _7, _8,	N, ...) N

#define DPSG_PP_RSEQ_N()	\
	8, 7, 6, 5, 4, 3, 2, 1

#define DPSG_PP_EXPAND(count, ...)	\
	DPSG_PP_EXPAND_##count (__VA_ARGS__)

#define DPSG_PP_EXPAND_1(a) a = 1
#define DPSG_PP_EXPAND_2(a, b) DPSG_PP_EXPAND_1(a), b = 1 << 1
#define DPSG_PP_EXPAND_3(a, b, c) DPSG_PP_EXPAND_2(a, b), c = 1 << 2
#define DPSG_PP_EXPAND_4(a, b, c, d) DPSG_PP_EXPAND_3(a, b, c), d = 1 << 3
#define DPSG_PP_EXPAND_5(a, b, c, d, e) DPSG_PP_EXPAND_4(a, b, c, d), e = 1 << 4
#define DPSG_PP_EXPAND_6(a, b, c, d, e, f) DPSG_PP_EXPAND_5(a, b, c, d, e), f = 1 << 5
#define DPSG_PP_EXPAND_7(a, b, c, d, e, f, g) DPSG_PP_EXPAND_6(a, b, c, d, e, f), g = 1 << 6
#define DPSG_PP_EXPAND_8(a, b, c, d, e, f, g, h) DPSG_PP_EXPAND_7(a, b, c, d, e, f, g), h = 1 << 7

#define DPSG_DEFINE_FLAG_(type, name, count, ...) 				\
	class name final : public									\
		dpsg::detail::BaseFlag<type, count> { 					\
		typedef BaseFlag<type, count> Base;						\
		public:													\
		enum {DPSG_PP_EXPAND(count, __VA_ARGS__)}; 				\
		name() : Base(Base::Count) {}							\
		name(type i) : Base(i) {} 								\
		~name() = default;										\
	};

#define DPSG_DEFINE_FLAG(type, name, ...) DPSG_DEFINE_FLAG_(type, name, DPSG_PP_NARG(__VA_ARGS__), __VA_ARGS__)

namespace dpsg { namespace detail
{

template<class T, T C>
class BaseFlag
{
	public:
		typedef T type;
		constexpr static T Count = C;

	protected:
		BaseFlag(type i) : _value(i) {} 
		BaseFlag() = default;
		virtual ~BaseFlag() = 0;
	
	private: 
		T _value;
};

}}

#endif
