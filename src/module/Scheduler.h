/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

namespace iris
{
  class Module ;
  class Scheduler
  {
    public:
      Scheduler() ;
      ~Scheduler() ;
      void schedule( Module* module, float priority ) ;
      void pulse() ;
      void stop() ;
    private:
      struct SchedulerData *scheduler_data ;
      SchedulerData& data() ;
      const SchedulerData& data() const ;
  };
}

