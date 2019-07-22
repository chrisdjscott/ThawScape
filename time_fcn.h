#ifndef _TIME_FCN_H_
#define _TIME_FCN_H_

#define FIRST_DAY 144  // May 25th
#define LAST_DAY 282   // Oct 10th, freezeup and minimum change after that


class time_fcn {
    public:
        /// \brief Create a time_fcn object
        /// \param year The initial year
        /// \param day The initial day
        /// \param hour The initial hour
        /// \param minute The initial minute
        /// \param end_year The final year
        /// \param end_day The final day
        time_fcn(int year = 0, int day = 0, int hour = 0, int minute = 0, int end_year = 0, int end_day = 0);

        /// \brief Determine whether to keep going (if we haven't reached the end yet)
        bool keep_going();

        /// \brief Get the current year
        int get_year();

        /// \brief Get the current day
        int get_day();

        /// \brief Get the current hour
        int get_hour();

        /// \brief Get the current minute
        int get_minute();

        /// \brief Get the end year
        int get_end_year();

        /// \brief Get the end day
        int get_end_day();

        /// \brief Advance the current time by the specified increment
        /// \param timestep The number of hours to move time forwards by
        void increment(int timestep);

        /// \brief Print some information about the current date time
        void print();

    private:
        int year;          ///< Current year
        int day;           ///< Current day. Everything works on 365 Julian Day system for now
        int hour;          ///< Current hour (24 hr clock)
        int minute;        ///< Current minute
        int LocalTime;     ///< Decimal Hour
        int UT;
        int end_year;      ///< Model end year
        int end_day;       ///< Model end day
};

#endif
