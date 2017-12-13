#ifndef MRT_TYPES_BOUNDED_BOUNDED_HPP_
#define MRT_TYPES_BOUNDED_BOUNDED_HPP_

#include <istream>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace mrt { namespace types { namespace bounded {
    enum class option {
        none,
        basic_arithmetic
    };

    template <typename t_bounded, typename t_constraint, option extra_operators = option::none> class bounded;
    template <typename t_bounded, typename t_constraint, option extra_operators = option::none> std::ostream& operator<<(std::ostream&, const bounded<t_bounded, t_constraint>&);
    template <typename t_bounded, typename t_constraint, option extra_operators = option::none> std::istream& operator>>(std::istream&, bounded<t_bounded, t_constraint>&);
    
    template<typename t_bounded, t_bounded lower_bound, t_bounded upper_bound>
    class range_constraint final {
        static_assert(lower_bound <= upper_bound, "Lower bound must be lower or equal to upper bound");
    
    public:
        constexpr bool operator()(const t_bounded& value) const noexcept {
            return value >= lower_bound && value <= upper_bound;
        }
    
        constexpr bool operator()(t_bounded&& value) const noexcept {
            return value >= lower_bound && value <= upper_bound;
        }
    };

    template<typename t_bounded, typename t_constraint, option extra_operators>
    class bounded {
        using has_arithmetic_op = typename std::enable_if<extra_operators == option::basic_arithmetic, bounded<t_bounded, t_constraint>>;

    public:
        using value_type = t_bounded;
    
        bounded() = default;
    
        bounded(t_bounded&& value) {
            assign(std::move(value));
        }
    
        auto operator=(const t_bounded& value) {
            assign(value);
        }
    
        auto operator=(t_bounded&& value) {
            assign(std::move(value));
        }
    
        auto operator->() noexcept {
            return &m_value;
        }
    
        auto operator->() const noexcept {
            return &m_value;
        }
    
        explicit operator t_bounded() const {
            return m_value;
        }
    
        t_bounded value() const {
            return m_value;
        }

    public:
        // Arithmetic operators
        
        template<typename t_operand, typename = has_arithmetic_op::type>
        auto operator+(t_operand&& op) {
            return bounded<t_bounded, t_constraint, extra_operators>(m_value + op);
        }
        
        template<typename = has_arithmetic_op::type>
        auto& operator++() {
            return assign(m_value + 1);
        }

        template<typename = has_arithmetic_op::type>
        auto operator++(int) {
            return bounded<t_bounded, t_constraint, extra_operators>(m_value + 1);
        }

        template<typename t_operand, typename = has_arithmetic_op::type>
        auto operator-(t_operand&& op) {
            return bounded<t_bounded, t_constraint, extra_operators>(m_value - op);
        }

        template<typename = has_arithmetic_op::type>
        auto& operator--() {
            return assign(m_value - 1);
        }

        template<typename = has_arithmetic_op::type>
        auto operator--(int) {
            return bounded<t_bounded, t_constraint, extra_operators>(m_value - 1);
        }

    public:
        // Streams
    
        friend std::ostream& operator<< <t_bounded, t_constraint, extra_operators> (std::ostream&, const bounded<t_bounded, t_constraint, extra_operators>&);
        friend std::istream& operator>> <t_bounded, t_constraint, extra_operators> (std::istream&, bounded<t_bounded, t_constraint, extra_operators>&);
    
    private:
        template<typename t_assign_value>
        auto& assign(t_assign_value&& value) {
            if (false == m_constraint(value)) {
                throw std::range_error("Value is out of constraint range.");
            }
    
            m_value = value;
            return *this;
        }
    
    private:
        t_constraint m_constraint;
        t_bounded m_value;
    };
    
    template<typename t_bounded, typename t_constraint, option extra_operators>
    std::ostream& operator<<(std::ostream& out, const bounded<t_bounded, t_constraint, extra_operators>& bounded_value) {
        return out << bounded_value.m_value;
    }
    
    template<typename t_bounded, typename t_constraint, option extra_operators>
    std::istream& operator>>(std::istream& in, bounded<t_bounded, t_constraint, extra_operators>& bounded_value) {
        typename bounded<t_bounded, t_constraint>::value_type raw_value;
        in >> raw_value;
        bounded_value.assign(raw_value);
        return in;
    }
    
    template<typename t_bounded, t_bounded lower_bound, t_bounded upper_bound, option extra_operators = option::none>
    using bounded_range = bounded<t_bounded, range_constraint<t_bounded, lower_bound, upper_bound>, extra_operators>;
} } }

#endif
