/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Manager.h
 * Author: jhendl
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

