#include "./player.h"

#include "../constants.h"
#include "../scene/scene_node.h"
#include "../scene/nodes/entities/main_character.h"

enum class MovementDirection {
  None,
  Left,
  Right
};

void Player::handleEvent(const sf::Event &t_event, CommandQueue &t_queue) {
  if (t_event.type == sf::Event::KeyPressed) {
    auto key = t_event.key.code;

    if (key == sf::Keyboard::Up || key == sf::Keyboard::W) {
      Command jump;
      jump.category = Category::Player;

      jump.action = [](SceneNode& node, sf::Time deltaTime) {
        MainCharacter &main_character = static_cast<MainCharacter &>(node);
        b2Vec2 currentVelocity = main_character.getVelocity();

        if (main_character.isOnGround()) {
          b2Vec2 velocity(currentVelocity.x, physics::JUMP_VELOCITY);
          main_character.setVelocity(velocity);
          main_character.setControlledVelocity(velocity);
        }
      };

      t_queue.push(jump);
    }
  }
}

void Player::handleInput(CommandQueue &t_queue) {
  auto speed = 0.f;
  auto direction = MovementDirection::None;
  auto jumping = false;
  auto pressed = sf::Keyboard::isKeyPressed;

  if (pressed(sf::Keyboard::D) || pressed(sf::Keyboard::Right)) {
    speed = physics::MAX_SPEED;
    direction = MovementDirection::Right;
  }
  else if (pressed(sf::Keyboard::A) || pressed(sf::Keyboard::Left)) {
    speed = -physics::MAX_SPEED;
    direction = MovementDirection::Left;
  }

  if (pressed(sf::Keyboard::W) || pressed(sf::Keyboard::Up)) {
    jumping = true;
  }

  Command move;
  move.category = Category::Player;
  move.action = [speed](SceneNode &node, sf::Time deltaTime) {
    MainCharacter &player = static_cast<MainCharacter &>(node);
    b2Vec2 current_velocity = player.getVelocity();
    auto acceleration = physics::ACCELERATION;
    auto x_velocity = current_velocity.x;
    auto x = acceleration * speed + (1 - acceleration) * x_velocity;
    auto velocity = b2Vec2(x, current_velocity.y);
    auto platform = player.getAttachedPlatform();

    x_velocity = player.getControlledVelocity().x;
    x = acceleration * speed + (1 - acceleration) * x_velocity;

    auto controlled_velocity = b2Vec2(x, current_velocity.y);

    if (platform) {
      auto x_platform_velocity = x + platform->getVelocity().x;
      b2Vec2 platform_velocity(x_platform_velocity, current_velocity.y);
      player.setVelocity(platform_velocity);
    }
    else {
      player.setVelocity(velocity);
    }
    player.setControlledVelocity(controlled_velocity);
  };

  Command animate;
  animate.category = Category::Player;
  animate.action = [direction](SceneNode &node, sf::Time deltaTime) {
    MainCharacter &player = static_cast<MainCharacter &>(node);
    switch (direction) {
      case MovementDirection::Left:
        player.turn(Entity::FacingDirection::Left);
        break;
      case MovementDirection::Right:
        player.turn(Entity::FacingDirection::Right);
        break;
      case MovementDirection::None:
        player.stopAnimation();
        break;
      default:
        break;
    };
  };

  Command jump;
  jump.category = Category::Player;
  jump.action = [jumping](SceneNode& node, sf::Time deltaTime) {
    MainCharacter &player = static_cast<MainCharacter &>(node);
    b2Vec2 current_velocity = player.getVelocity();
    auto y_velocity = std::min(physics::MAX_JUMP_VELOCITY, current_velocity.y);

    if (!jumping && !player.isOnGround()) {
      player.setVelocity(b2Vec2(current_velocity.x, y_velocity));
    }
  };

  t_queue.push(jump);
  t_queue.push(move);
  t_queue.push(animate);
}