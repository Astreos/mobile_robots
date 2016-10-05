module ExternalMBS

  use iso_c_binding
  use overall

  implicit none

  private

! * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
! Bindings

  interface
    subroutine f_initialize() bind(c, name="mbs_lmgc_initialize")
    end subroutine f_initialize
  end interface

  interface
    subroutine f_finalize() bind(c, name="mbs_lmgc_finalize")
    end subroutine f_finalize
  end interface

  interface
    subroutine f_increment(tbegin) bind(c, name="mbs_lmgc_increment")
      import c_double
      real(c_double), intent(in), value :: tbegin
    end subroutine f_increment
  end interface

  interface
    subroutine f_compute_free_vlocy(h, theta) bind(c, name="mbs_lmgc_compute_free_vlocy")
      import c_double
      real(c_double), intent(in), value :: h
      real(c_double), intent(in), value :: theta
    end subroutine f_compute_free_vlocy
  end interface

  interface
    subroutine f_update_nodes_3D(i_mbs, coorTT, localFrameTT) bind(c, name="mbs_lmgc_update_nodes_3D")
      import c_int, c_ptr
      integer(c_int), value :: i_mbs
      type(c_ptr), value :: coorTT
      type(c_ptr), value :: localFrameTT
      
    end subroutine f_update_nodes_3D
  end interface
  
  interface
    subroutine f_update_nodes_2D(i_mbs, coorTT) bind(c, name="mbs_lmgc_update_nodes_2D")
      import c_int, c_ptr
      integer(c_int), value :: i_mbs
      type(c_ptr), value :: coorTT
      
    end subroutine f_update_nodes_2D
  end interface

  interface
    subroutine f_nullify_reac(i_mbs) bind(c, name="mbs_lmgc_nullify_reac")
      import c_int
      integer(c_int), value :: i_mbs
    end subroutine f_nullify_reac
  end interface

  interface
    subroutine f_nullify_rAux(i_mbs) bind(c, name="mbs_lmgc_nullify_rAux")
      import c_int
      integer(c_int), value :: i_mbs
    end subroutine f_nullify_rAux
  end interface


  interface
    subroutine f_add_reac(i_mbs, i_node, reac) bind(c, name="mbs_lmgc_add_reac")
      import c_int, c_ptr, c_double
      integer(c_int), value :: i_mbs
      integer(c_int), value :: i_node
      real(c_double), dimension(6) :: reac
    end subroutine f_add_reac
  end interface

  interface
    subroutine f_add_rAux(i_mbs, i_node, reac) bind(c, name="mbs_lmgc_add_rAux")
      import c_int, c_ptr, c_double
      integer(c_int), value :: i_mbs
      integer(c_int), value :: i_node
      real(c_double), dimension(6) :: reac
    end subroutine f_add_rAux
  end interface

  interface
    subroutine f_nullify_vFree(i_mbs) bind(c, name="mbs_lmgc_nullify_vFree")
      import c_int
      integer(c_int), value :: i_mbs
    end subroutine f_nullify_vFree
  end interface

  interface
    subroutine f_nullify_vAux(i_mbs) bind(c, name="mbs_lmgc_nullify_vAux")
      import c_int
      integer(c_int), value :: i_mbs
    end subroutine f_nullify_vAux
  end interface

  interface
    subroutine f_comp_vAux(i_mbs) bind(c, name="mbs_lmgc_comp_vAux")
      import c_int
      integer(c_int), value :: i_mbs
    end subroutine f_comp_vAux
  end interface

  interface
    subroutine f_get_vBeg(i_mbs, i_node, vlocy) bind(c, name="mbs_lmgc_get_vBeg")
      import c_int, c_ptr, c_double
      integer(c_int), value :: i_mbs
      integer(c_int), value :: i_node
!      type(c_ptr), value :: vlocy
      real(c_double), dimension(6) :: vlocy
    end subroutine f_get_vBeg
  end interface

  interface
    subroutine f_get_vFree(i_mbs, i_node, vlocy) bind(c, name="mbs_lmgc_get_vFree")
      import c_int, c_ptr, c_double
      integer(c_int), value :: i_mbs
      integer(c_int), value :: i_node
      real(c_double), dimension(6) :: vlocy
    end subroutine f_get_vFree
  end interface

  interface
    subroutine f_get_vAux(i_mbs, i_node, vlocy) bind(c, name="mbs_lmgc_get_vAux")
      import c_int, c_ptr, c_double
      integer(c_int), value :: i_mbs
      integer(c_int), value :: i_node
      real(c_double), dimension(6) :: vlocy
    end subroutine f_get_vAux
  end interface


  interface
    subroutine f_compute_dof(h, theta) bind(c, name="mbs_lmgc_compute_dof")
      import c_double
      real(c_double), intent(in), value :: h
      real(c_double), intent(in), value :: theta
    end subroutine f_compute_dof
  end interface


  interface
    subroutine f_update_dof() bind(c, name="mbs_lmgc_update_dof")
    end subroutine f_update_dof
  end interface


!EXAMPLE
!  
!  interface
!    subroutine f_compute_velocity(i_mbs,reac,vlocy,nb_nodes,nb_dofs) bind(c, name="mbs_lmgc_compute_velocity")
!      import c_int, c_ptr
!      integer(c_int), intent(in), value :: i_mbs
!      integer(c_int), intent(in), value :: nb_nodes
!      integer(c_int), intent(in), value :: nb_dofs
!      type(c_ptr), value :: reac
!      type(c_ptr), value :: vlocy
!    end subroutine f_compute_velocity
!  end interface

! * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
! * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
! functions to call from LMGC90

  public initialize, finalize, increment,                     &
         compute_free_vlocy, comp_vlocy,                      &
         nullify_reac, nullify_vlocy,                         &
         compute_dof, update_dof,                             &
         update_nodes_3D, add_reac_3D, get_vlocy_3D,          &
         update_nodes_2D, add_reac_2D, get_vlocy_2D

  contains


  subroutine initialize()
    implicit none

    ! il faudrait charger robotran ici (a voir)
    call f_initialize()
    
  end subroutine
  

  ! debut boucle en temps
  
  subroutine increment(tbegin)
    implicit none

    !> value of simulation time at the begining of the time step
    real(kind=8), intent(in) :: tbegin

    ! on avance d'un pas de temps
    call f_increment(tbegin)
    
  end subroutine 

  ! debut boucle Newton 
  
  subroutine compute_free_vlocy(h, theta)
    implicit none
    
    !> time step size
    real(kind=8), intent(in) :: h
    !> theta parameter for the theta-integration scheme
    real(kind=8), intent(in) :: theta
    
    
    call f_compute_free_vlocy(h, theta)
    
  end subroutine
  
  subroutine update_nodes_3D(i_mbs, coorTT, localFrameTT, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs
    integer(kind=4), intent(in) :: storage 
    real(kind=8), dimension(:,:), pointer :: coorTT
    real(kind=8), dimension(:,:), pointer :: localFrameTT
    
                             !123456789012345678901234
    character(len=24) :: IAM='ext_mbs::update_nodes_3D'
    character(len=80) :: cout
    ! WARNING : index must be adapted fortran:1 <-> C:0
    
    ! OL: TODO => Use the storage to store the Jredsens(q or qm)
    call f_update_nodes_3D(i_mbs-1, c_loc(coorTT(1,1)), c_loc(localFrameTT(1,1)))
    
  end subroutine
  
  subroutine update_nodes_2D(i_mbs, coorTT, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs
    integer(kind=4), intent(in) :: storage 
    real(kind=8), dimension(:,:), pointer :: coorTT
    
                             !123456789012345678901234
    character(len=24) :: IAM='ext_mbs::update_nodes_2D'
    character(len=80) :: cout
    ! WARNING : index must be adapted fortran:1 <-> C:0
    
    ! OL: TODO => Use the storage to store the Jredsens(q or qm)
    !          => Implement 2D update nodes in Robotran
    call f_update_nodes_2D(i_mbs-1, c_loc(coorTT(1,1)))
    
  end subroutine

  subroutine nullify_reac(i_mbs, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs    
    integer(kind=4), intent(in) :: storage
    
                             !123456789012345678901
    character(len=21) :: IAM='ext_mbs::nullify_reac'
    character(len=80) :: cout
    
    ! WARNING : index must be adapted fortran:1 <-> C:0
    
    SELECT CASE (storage)
      CASE (iReac_)
          call f_nullify_reac(i_mbs-1);
      CASE (iRaux_)
          call f_nullify_rAux(i_mbs-1);
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT
    
  end subroutine


  subroutine add_reac_3D(i_mbs, i_node, reac, storage, LFTT)
    implicit none
    
    integer(kind=4), intent(in)    :: i_mbs    
    integer(kind=4), intent(in)    :: i_node
    integer(kind=4), intent(in)    :: storage
    real(kind=8)   , dimension(6)  :: reac
    !> Reaction with Forces expressed in global frame and torque in body frame
    real(kind=8)   , dimension(6)  :: reac_glob
    !> reac vector with torques expressed in global frame.
    !> reac is not modified because it's intent(in) in mod_mbs.f90
    real(kind=8)   , dimension(3,3):: LFTT
    !> Matrix to pass from global frame to body frame
    
                             !123456789012345678901
    character(len=20) :: IAM='ext_mbs::add_reac_3D'
    character(len=80) :: cout
    
    ! Expressing torque in global frame
    reac_glob(:) = reac(:)
    reac_glob(4) = LFTT(1,1)*reac(4) + LFTT(1,2)*reac(5) + LFTT(1,3)*reac(6)
    reac_glob(5) = LFTT(2,1)*reac(4) + LFTT(2,2)*reac(5) + LFTT(2,3)*reac(6)
    reac_glob(6) = LFTT(3,1)*reac(4) + LFTT(3,2)*reac(5) + LFTT(3,3)*reac(6)
    
    ! WARNING : index must be adapted fortran:1 <-> C:0

    SELECT CASE (storage)
      CASE (iReac_)
          call f_add_reac(i_mbs-1, i_node-1, reac_glob)
      CASE (iRaux_)
          call f_add_rAux(i_mbs-1, i_node-1, reac_glob)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT

   
  end subroutine
  
    subroutine add_reac_2D(i_mbs, i_node, reac, storage)
    implicit none
    
    integer(kind=4), intent(in)    :: i_mbs    
    integer(kind=4), intent(in)    :: i_node
    integer(kind=4), intent(in)    :: storage
    real(kind=8)   , dimension(3)  :: reac
    !> Reaction with Forces along X and Y an torque around Z expressed in global frame
    real(kind=8)   , dimension(6)  :: reac_glob
    !> Same vector as reac but completed in 3D : [Fx, Fy, 0, 0, 0, Tz]
    
                             !123456789012345678901
    character(len=20) :: IAM='ext_mbs::add_reac_2D'
    character(len=80) :: cout
    reac_glob(1)=reac(1)
    reac_glob(2)=reac(2)
    reac_glob(6)=reac(3)
    reac_glob(3:5)=0.0
    ! WARNING : index must be adapted fortran:1 <-> C:0

    SELECT CASE (storage)
      CASE (iReac_)
          call f_add_reac(i_mbs-1, i_node-1, reac_glob)
      CASE (iRaux_)
          call f_add_rAux(i_mbs-1, i_node-1, reac_glob)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT

   
  end subroutine


  subroutine nullify_vlocy(i_mbs, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs    
    integer(kind=4), intent(in) :: storage
    
   
    
                             !1234567890123456789012
    character(len=22) :: IAM='ext_mbs::nullify_vlocy'
    character(len=80) :: cout

    ! WARNING : index must be adapted fortran:1 <-> C:0

    SELECT CASE (storage)
      CASE (iVfree)
          call f_nullify_vFree(i_mbs-1)
      CASE (iVaux_)
          call f_nullify_vAux(i_mbs-1)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT
    
    
  end subroutine


  subroutine comp_vlocy(i_mbs, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs    
    integer(kind=4), intent(in) :: storage
    

                             !1234567890123456789012
    character(len=19) :: IAM='ext_mbs::comp_vlocy'
    character(len=80) :: cout

    ! WARNING : index must be adapted fortran:1 <-> C:0

    SELECT CASE (storage)
      CASE (iVaux_e_invM_t_Raux_)
          call f_comp_vAux(i_mbs-1)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT

    
  end subroutine


  subroutine get_vlocy_3D(i_mbs, i_node, vlocy, storage, LFTT)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs    
    integer(kind=4), intent(in) :: i_node
    integer(kind=4), intent(in) :: storage
    
    real(kind=8)   , dimension(6)  :: vlocy
    !> Velocity with linear velocities expressed in global frame and angular in body frame
    real(kind=8)   , dimension(6)  :: vlocy_glob
    !> Linear and angular velocities expressed in global frame
    real(kind=8)   , dimension(3,3):: LFTT
    !> Matrix to pass from global frame to body frame
    
                             !1234567890123456789012
    character(len=21) :: IAM='ext_mbs::get_vlocy_3D'
    character(len=80) :: cout
   
    ! WARNING : index must be adapted fortran:1 <-> C:0
    vlocy_glob = 0.0
    SELECT CASE (storage)
      CASE (iVbeg_)
          call f_get_vBeg(i_mbs-1, i_node-1, vlocy_glob)
      CASE (iVfree)
          call f_get_vFree(i_mbs-1, i_node-1, vlocy_glob)
      CASE (iVaux_)
          call f_get_vAux(i_mbs-1, i_node-1, vlocy_glob)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT
    
    ! Express the angular velocities in body frame
    vlocy(1:6) = vlocy_glob(1:6)
    vlocy(4) = LFTT(1,1)*vlocy_glob(4) + LFTT(2,1)*vlocy_glob(5) + LFTT(3,1)*vlocy_glob(6)
    vlocy(5) = LFTT(1,2)*vlocy_glob(4) + LFTT(2,2)*vlocy_glob(5) + LFTT(3,2)*vlocy_glob(6)
    vlocy(6) = LFTT(1,3)*vlocy_glob(4) + LFTT(2,3)*vlocy_glob(5) + LFTT(3,3)*vlocy_glob(6)
    
  end subroutine

  subroutine get_vlocy_2D(i_mbs, i_node, vlocy, storage)
    implicit none
    
    integer(kind=4), intent(in) :: i_mbs    
    integer(kind=4), intent(in) :: i_node
    integer(kind=4), intent(in) :: storage
    
    real(kind=8)   , dimension(3)  :: vlocy
    !> Velocity with Vx, Vy and omegaZ in global frame
    real(kind=8)   , dimension(6)  :: vlocy_glob
    !> Same vector as vlocy but completed in 3D : [Vx, Vy, Vz, Om_X, Om_Y, Om_Z]
    
                             !1234567890123456789012
    character(len=21) :: IAM='ext_mbs::get_vlocy_2D'
    character(len=80) :: cout
   
    ! WARNING : index must be adapted fortran:1 <-> C:0
    vlocy_glob = 0.0
    SELECT CASE (storage)
      CASE (iVbeg_)
          call f_get_vBeg(i_mbs-1, i_node-1, vlocy_glob)
      CASE (iVfree)
          call f_get_vFree(i_mbs-1, i_node-1, vlocy_glob)
      CASE (iVaux_)
          call f_get_vAux(i_mbs-1, i_node-1, vlocy_glob)
      CASE DEFAULT
          write(cout,'(A,I5)') 'Unsupported flag value ',storage
          call FATERR(IAM,cout)
    END SELECT
    
    ! Retrieve the velocities in the plane X,Y and around Z axis
    vlocy(1:2) = vlocy_glob(1:2)
    vlocy(3)   = vlocy_glob(6)
    
  end subroutine


  subroutine compute_dof(h, theta)
    implicit none
    
    !> time step size
    real(kind=8), intent(in) :: h
    !> theta parameter for the theta-integration scheme
    real(kind=8), intent(in) :: theta
    
    
    call f_compute_dof(h, theta)
    
  end subroutine


  ! fin boucle newton 
  
  subroutine update_dof()
    implicit none

    ! calcul nouvelle configuration 

    ! update integration
    
    call f_update_dof()
  
  end subroutine 

  !fin boucle en temps
  
  subroutine finalize()
    implicit none 

    ! on termine le calcul
    call f_finalize()
    
  end subroutine 


! EXAMPLE
!  
!   !> \brief Call of compute free velocity and detection configuration in robotran
!  subroutine compute_free_vlocy(i_mbs,vlocy)
!    implicit none
!    !> mbs index
!    integer(kind=4), intent(in) :: i_mbs
!    !> velocity by robotran
!    real(kind=8), pointer :: vlocy
!    !
!    type(c_ptr) :: reac
!
!    ! in the case of free_vlocy, reac is null
!    reac = c_null_ptr
!
!    ! calcul vitesse libre
!    ! ...
!    ! on donne coorTT, localFrameTT
!
!    call f_compute_velocity(i_mbs,reac,c_loc(vlocy(1,1)),size(vlocy,2),size(vlocy,1))
!
!  end subroutine 


end module
