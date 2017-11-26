#ifndef MRT_BOUNDED_HPP_
#define MRT_BOUNDED_HPP_

#include <cassert>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace mrt { namespace types { namespace bounded {   

    template <typename t_bounded, typename t_constraint> class bounded;
    template <typename t_bounded, typename t_constraint> std::ostream& operator<<(std::ostream&, const bounded<t_bounded, t_constraint>&);
    template <typename t_bounded, typename t_constraint> std::istream& operator>>(std::istream&, const bounded<t_bounded, t_constraint>&);

    template<typename t_bounded, t_bounded min, t_bounded max>
    class range_constraint {
        static_assert(min <= max, "min must be lower or equal to max");

    public:
        using value_type = t_bounded;

        bool operator()(const t_bounded& value) const noexcept {
            return value >= min && value <= max;
        }

        bool operator()(t_bounded&& value) const noexcept {
            return value >= min && value <= max;
        }
    };

    template<typename t_bounded, typename t_constraint>
    class bounded {
    public:
        bounded() = default;

        bounded(t_bounded&& value) {
            assign_if_valid(std::move(value));
        }

        auto operator=(const t_bounded& value) {
            assign_if_valid(value);
        }

        auto operator=(t_bounded&& value) {
            assign_if_valid(std::move(value));
        }
        
        auto operator->() { 
            return &m_value; 
        }

        auto operator->() const { 
            return &m_value;
        }
   
        t_bounded value() const {
            return m_value;
        }

        friend std::ostream& operator<< <t_bounded, t_constraint>(std::ostream&, const bounded<t_bounded, t_constraint>&);
        friend std::istream& operator>> <t_bounded, t_constraint>(std::istream&, const bounded<t_bounded, t_constraint>&);

    private:
        void assign_if_valid(const t_bounded& value) {
            if (false == m_constraint(value)) {
                assert(false && "Value is out of constraint range.");
                throw std::range_error("Value is out of constraint range.");
            } else {
                m_value = value;
            }
        }

    private:
        t_constraint m_constraint;
        t_bounded m_value;
    };

    template<typename t_bounded, typename t_constraint>
    std::ostream& operator<<( std::ostream& out, const bounded<t_bounded, t_constraint>& bounded_value) {
        return out << bounded_value.m_value;
    }

    template<typename t_bounded, typename t_constraint>
    std::istream& operator>>(std::istream& in, const bounded<t_bounded, t_constraint>& bounded_value) {
        typename bounded<t_bounded, t_constraint>::value_type raw_value;
        in >> raw_value;
        bounded_value.assign_if_valid(raw_value);
        return in;
    }

    template<typename t_bounded, t_bounded min, t_bounded max> 
    using bounded_range = bounded<t_bounded, range_constraint<t_bounded, min, max>>;

} } }

#endif
