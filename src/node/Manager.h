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
/* 
 * File:   Manager.h
 * Author: Jordan Hendl
 *
 * Created on July 10, 2020, 6:04 AM
 */

#ifndef KARMA_MANAGER_H
#define KARMA_MANAGER_H

namespace karma
{
  class Manager
  {
    public:
      Manager() ;
      ~Manager() ;
      void initialize( const char* mod_path, const char* configuration_path ) ;
      void start() ;
      void stop() ;
      void shutdown() ;
    private:
      struct ManagerData* man_data ;
      ManagerData& data() ;
      const ManagerData& data() const ;
  };
}

#endif /* MANAGER_H */

